/*
 * WebServer.c
 * This file is part of Vallaurisoft
 *
 * Copyright (C) 2014 - Thread
 *
 * Vallaurisoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Vallaurisoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vallaurisoft; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#define MAX_BUFFER 4096

#include <iostream>

using namespace std;

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "errore.h"
#include "Socket.hpp"
#include "Address.hpp"
#include "List.hpp"
#include "TCP.hpp"
#include "WebPage.h"
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define LEN_MESSAGES 10


/*struttura thread param*/
typedef struct {   int thread_num;
		   Conn_Server connection;} Thread_param;



/*routine del thread*/
void* ExecuteThis( void* conn_id) {

	Thread_param * param;

	param =(Thread_param) conn_id;

	/*Riceve la richiesta dal client*/
	msgrec = conn_id->connection->ricevi();

	/*Richiesta*/
	printf("#Richiesta#\n%s\n--------------\n\n",msgrec ); 

	msg=stringConcat(header,CheckHttpRequest(msgrec));

	/*Invia la risposta*/
	conn_id->connection->invia(msg);

	printf("#Risposta#\n%s\n---------------\n\n",msg);  
	
	pthread_exit(NULL);
}


/*int str2int(char* s){
    int ret;
    for(ret=0;*s;s++)
	    ret=ret*10+*s-'0';
    return(ret);
}*/

int main(int argc, char** argv) {
  
	pthread_t * my_threads;
	Thread_param * param;

	int ret_code;
	int i;
	int num_threads;

	Thread_param* curr_param;

	char* msgrec;
	char* msg;
	char* header = "HTTP/1.1 200 OK\n\
			Date: Mon, 23 May 2005 22:38:34 GMT\n\
			Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\n\
			Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\n\
			Accept-Ranges: bytes\n\
			Connection: close\n\
			\n";

	int server_port;

	if (argc !=2) {	
		printf("USAGE:%s PORT\n",argv[0]);
		return -1; 
	}
	/*-----------------------apro il server------------------------------------------------------------------------------*/

	server_port = atoi(argv[1]);

	/*Creo un client singolo su cui operare*/
	Conn_Server* client;	
	Address* addr = new Address("127.0.0.1",server_port);
	ServerTCP* server = new ServerTCP(server_port);

	/*-----------------------alloco le zone di memoria-------------------------------------------------------------------*/
	
	/*creo la zona di memoria per i thread*/
	my_threads = (pthread_t*) malloc (sizeof (pthread_t));

	/*creo la zona di memoria per i parametri dei thread*/
	param = (Thread_param*) malloc (sizeof (Thread_param));

	/*-----------------------accetto le connesioni, allargo la zona di memoria e azzero il contatore---------------------*/

	i=0;
	while ( i< MAX_CONN) {
			if(client = server->accetta(addr))
			{
				/*-------------------allargo la memoria------------------------------------------------------*/
				
				/*rialloco la memoria allargandola aggiongendo un nuovo elemento*/
				my_threads = realloc(my_threads, sizeof (pthread_t)*i+1);

				/*rialloco la memoria allargandola aggiungendo un nuovo elmento*/
				param = realloc(param, sizeof (Thread_param)*i+1);

				/*passo il valore del puntatore per avere in current param i dati correnti*/
				curr_param = param+i;

				/*-------------------inserisco i dati nella struct-------------------------------------------*/
				
				/*definisco il numero del thread*/
				curr_param->thread_num = i;

				/*metto nel campo connection il risultato dell'accept*/
				curr_param->connection = client;

				/*-------------------istanzio il thread------------------------------------------------------*/

				/*istanzio il thread*/
				ret_code = pthread_create( (my_threads + i), NULL, ExecuteThis , (void*) curr_param);

				/*if di controllo*/
				if (ret_code) {
							printf("Creazione Thread Fallita \nerror code:%d\n",i);
							printf("%d:(%s)\n",errno,strerror(errno));
					      }
				else { 
					printf("creato il thread numero %d\n",i);
					i++;
					num_threads++;
				     }
			}
		}

	/*----------------------attendo che i thread terminino---------------------------------------------------------------*/
	for(i=0;i<num_threads;i++)
		pthread_join(*(my_threads + i),NULL);

	/*----------------------libero le zone di memoria e distruggo le strutture-------------------------------------------*/
	free(param);
	free(my_threads);
	delete(addr);
	delete(server);
	delete(client);
	
return(0);
}

