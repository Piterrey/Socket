/*
 * SocketTCP.c
 * This file is part of VallauriSoft
 *
 * Copyright (C) 2014 - Ghisolfo
 *
 * VallauriSoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * VallauriSoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VallauriSoft; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONN 42
#define MYSELF_IP "0.0.0.0"
#define MAX_BUFFER 4096

void errore(char*,int);

typedef struct sockaddr_in Addr;

int main (int argc, char *argv[])
{
    char buffer[MAX_BUFFER+1], *msg;
    int port, sockID,len_addr, ret_code, i, connID, len_msg;
    Addr *client_addr , *myself_addr;
    
    char response[]="HTTP/1.1 200 OK\n\
Date: Mon, 23 May 2005 22:38:34 GMT\n\
Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\n\
Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\n\
ETag: \"3f80f-1b6-3e1cb03\"\n\
Content-Type: text/html; charset=UTF-8\n\
Accept-Ranges: bytes\n\
Connection: close\n\
\n\
<html>\n\
<head>\n\
  <title>Esempio</title>\n\
  <link rel=\"stylesheet\" media=\"screen\" href=\"/213/213.css?v=8may2013\">\n\
</head>\n\
<body>\n\
  Hello World, this is a very simple HTML document.\n\
</body>\n\
</html>\n";
    
    if(argc != 2) 
    {
        printf("USAGE: %s PORT:\n",argv[0]);
        return (-1);
    }
    
    port = atoi(argv[1]);
    //msg = argv[2];
    
    printf("Creo un socket...\n");
    sockID = socket(AF_INET, SOCK_STREAM,0); //apro il socket
    if(sockID <= 0) errore("ERRORE nella creazione del socket\n",-2);
    
    
    printf("Associo il socket alla porta %d... \n", port);
    
    myself_addr = (Addr*)malloc(sizeof(Addr));
    myself_addr -> sin_family = AF_INET;
    inet_aton(MYSELF_IP, &(myself_addr -> sin_addr));
    myself_addr -> sin_port = htons(port);
    for(i=0;i<8;i++) myself_addr -> sin_zero[i] = 0;
    
    len_addr = sizeof(Addr);
    ret_code = bind(sockID, (struct sockaddr *)myself_addr,(socklen_t) len_addr); //assegno qualche porta
    if(ret_code)     errore("ERRORE nella assegnazione della porta\n",-3);
    
    printf("Indico il numero di connesioni....\n");
    ret_code = listen(sockID, MAX_CONN); // si dice quante connessioni può accettare
    if(ret_code)  errore("ERRORE nell'assegnare il numero massimo di connessioni\n",-4);
    
    printf("Attendo che il client mi contatti per stabilire una connessione....\n");
    len_addr = sizeof(Addr);
    client_addr = (Addr*) malloc(sizeof(Addr));
    
    connID = accept(sockID,(struct sockaddr*) client_addr, (socklen_t* )&len_addr); //apro la connessione
    if(connID <= 0) errore("ERRORE nell'accettare una connessione\n",-5);
    
    printf("Accettata una connessione da %s : %d ...\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    
    printf("Attendo che il client mi dica qualcosa....\n");
    ret_code = recv(connID, buffer, MAX_BUFFER, 0); //ricevo i dati
    
    if(ret_code <= 0)   errore("ERRORE durante la ricezione dal client\n",-6);
    buffer[ret_code]='\0';
    printf("Ho ricevuto : '%s' dal client.\n", buffer);
    
    /*printf("Rispondo al client inviandogli '%s'.... \n",msg);
    len_msg = strlen(msg)+1;
    ret_code = send(connID, msg, len_msg, 0); //mando i dati
    if(ret_code != len_msg) errore("ERRORE durante l'invio della risposta al client\n",-7);*/
    
    printf("Rispondo al client inviandogli '%s'.... \n",response);
    len_msg = strlen(response)+1;
    ret_code = send(connID, response, len_msg, 0); //mando i dati
    if(ret_code != len_msg) errore("ERRORE durante l'invio della risposta al client\n",-8);
    
    printf("Chiudo la connessione-- \n");
    close(connID);
    
    printf("Chiudo il Socket....\n"); 
    shutdown(sockID, SHUT_RDWR); //chiudo
    
    free(client_addr);
    free(myself_addr);
    return 0;
    
}



void errore(char*str,int n)
{
    printf(str);
    printf("%s(%d)\n",strerror(errno),errno);
    exit(n);
}






