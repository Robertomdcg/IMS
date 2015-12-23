
#include "soapH.h"
#include "imsService.nsmap"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "serverFiles.h"

#define DEBUG_MODE 1

//Usuarios registrados en el servidor
UserList *userlist;


int main(int argc, char **argv){ 

  int m, s;
  struct soap soap;

  	if (argc < 2) {
    	printf("Usage: %s <port>\n",argv[0]); 
		exit(-1); 
  	}

	// Init environment
  	soap_init(&soap);

	// Bind to the specified port	
	m = soap_bind(&soap, NULL, atoi(argv[1]), 100);

	// Check result of binding		
	if (m < 0) {
  		soap_print_fault(&soap, stderr); exit(-1); 
	}

	//Inicializar servidor
	userlist = (UserList*)malloc(sizeof(UserList));
	serverInit(userlist);


	// Listen to next connection
	while (1) { 
		// accept
	  	s = soap_accept(&soap);    
	  	if (s < 0) {
			soap_print_fault(&soap, stderr); exit(-1);
	  	}
		// Execute invoked operation
	  	soap_serve(&soap);
		// Clean up!
	  	soap_end(&soap);
	}

  return 0;
}



int procesaMensaje(struct Message message) {
	char usuario[200];	
	switch(message.operation) {
		case 1: //Alta usuario
			strcpy(usuario, message.name);
			printf("%s\n", usuario);
			break;
		case 2: //Baja usuario
			
			break;
		default:break;
	}

	return 0;
}


int ims__sendMessage (struct soap *soap, struct Message myMessage, int *result){
	*result = procesaMensaje(myMessage);	
	//printf ("Received by server: \n\tusername:%s \n\tmsg:%s\n toperation:%d\n", myMessage.name, myMessage.msg, myMessage.operation);
	return SOAP_OK;
}


int ims__receiveMessage (struct soap *soap, struct Message *myMessage){

	// Allocate space for the message field of the myMessage struct then copy it
	myMessage->msg = (xsd__string) malloc (IMS_MAX_MSG_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMessage->msg, 0, IMS_MAX_MSG_SIZE);
	strcpy (myMessage->msg, "Invoking the remote function receiveMessage simply retrieves this standard message from the server"); // always same msg

	// Allocate space for the name field of the myMessage struct then copy it
	myMessage->name = (xsd__string) malloc (IMS_MAX_NAME_SIZE);
	// Not necessary with strcpy since uses null-terminated strings
	// memset(myMessage->name, 0, IMS_MAX_NAME_SIZE);  
	strcpy(myMessage->name, "aServer");	

	return SOAP_OK;
}

/*
int userLogin(UserList* luser,char* nick,char* pass){
	int found = 0;
	int i = 0;
	User *user = NULL;
	while(i < luser->numUser && found == 0){
		if(strcmp(nick,luser->listU[i]->nick) == 0){
			found = 1;
			user = luser->listU[i];
		}
		i++;
	}
	if(found == 1 && strcmp(pass,user->pass) == 0){
		user->online = 1;
		return 0;
	}

	return -1;
}

int ims__login(struct soap *soap, char* username, char* password, int *error){
	*error = userLogin(userlist,nick,pass);

	if(DEBUG_MODE && *error == 0){
		printf("ims__userLogin -> Se ha logueado: %s\n",username);
	}
	return SOAP_OK;
}
*/

int ims__registerUser(struct soap *soap, char* username, char* password, int *error) {
	
	addUser(userlist, username, password);
	
	
	
	return SOAP_OK;
}

