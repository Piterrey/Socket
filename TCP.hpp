/*
 * TCP.hpp
 * This file is part of Vallaurisoft
 *
 * Copyright (C) 2014 - Ferrari
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

class Socket {
	private: int sock_id;
	public:	 Socket(int mode){
				    sock_id = socket(AF_INET,mode,0);
				 };
		 ~Socket(){
			      shutdown(sock_id, SHUT_RDWR);
			  };
		 int getSockId(){
		 			return sock_id;
		 		};
};

class socketUDP:socketUDP:Socket(SOCK_DGRAM){
	private:	int ret_code;
	public:		
			SocketUDP(){
					
				   };
			~SocketUDP()
			bool invia(char* msg,Address* dest){
								dest_addr = dest->get_addr();
								ret_code = send_to( sock_id,msg,strlen(msg)+1,0,(struct sockaddr*) dest_addr,(socklen_t)sizeof(dest_addr));
								if(ret_code<0) return false;
								      else return true;
							   };
			char* ricevi(Address* sender,int maxbuffer){
							  char buffer[maxbuffer+1];
							  struct sockaddr_in mitt_addr;
							  int len_addr;
							  int ret_code;
							  
							  len_addr = sizeof(struct sockaddr_in);
							  ret_code = recvfrom(sock_id,buffer,maxbuffer,0,(struct sockaddr*)&mitt_addr(socklen_t*)&len_addr);
							  if(ret_code <= 0) return NULL;   
							  
							  sender ->set_addr(&mitt_addr);
							  buffer[ret_code] = '\0';
							  return strdup(buffer);
						     };
}; 

class ClientUDP:socketUDP{
	private:
	public:		ClientUDP();
			~ClientUDP();

};

class ServerUDP:socketUDP{
	private:
	public:		ServerUDP(int port){
						Address*myself;
						struct sockaddr_in* addr;
						if(sock_id < 0) errore("Socket non aperto!\n",-1);
						
						myself = new Address("0.0.0.0",port);
						addr = myself->get_addr();
						
						if( bind (sock_id,
							(struct sockaddr*) addr,
							(socklen_t) sizeof(struct sockaddr)))
									return false;

						delete(myself);
						free(addr);
					    };
			~ServerUDP();

};

class SocketTCP::SocketTCP:Socket(SOCK_DGRAM) {
	public:		
			SocketTCP();
			~SocketTCP(){
					shutdown(sock_id, SHUT_RDWR);
				    };
}; 

class ClientTCP:SocketTCP{	
	private:	Connessione* connessione;
			int ret_code;
	public:		ClientTCP();
			~ClientTCP();
			bool connetti(Address* server){
							connessione=connect(sock_id,server,sizeof(server);
							if(connessione<0) return false;
							else return true;
						      };
			bool close_connessione(){
						    ret_code=close(connessione);
						    if(ret_code<0) return false;
							else return true;
						};
			bool invia(char* msg){
						 ret_code=send(connessione, msg, strlen(msg), 0);
						 if(ret_code<0) return false;
							else return true;
					     };
			char* ricevi(int maxbuffer){
					  char buffer[maxbuffer+1];
					  ret_code = recv(connessione, buffer, maxbuffer, 0); 
					  if(ret_code <= 0) return null;
					  buffer[ret_code]='\0';
					  return strdup(buffer);
				      };
};
class ServerTCP:SocketTCP
	private:	Lista* lista_connessioni;
	public:		ServerTCP(int port){
						myself_addr = (Addr*)malloc(sizeof(Addr));
						myself_addr -> sin_family = AF_INET;
						inet_aton("0.0.0.0", &(myself_addr -> sin_addr));
						myself_addr -> sin_port = htons(port);
						for(i=0;i<8;i++) myself_addr -> sin_zero[i] = 0;
						
						len_addr = sizeof(sockaddr_in);
						ret_code = bind(sockID, (struct sockaddr *)myself_addr,(socklen_t) len_addr);
					   };//bind()-listen()
			~ServerTCP();
			Connessione* accettata();
			void close_tutte_connessioni();
			void invia_a_tutti(char* msg);
};

class Nodo{
	private:	Nodo* _next;
	public:		Nodo(Nodo* next);
			~Nodo();		
			void set_next(Nodo*);
			Nodo* get_next();
};

class Iterator;

class Lista{
	private:	Nodo* first;
			void remove_Node(Nodo* curr) {
							if (curr){
								remove_Node(curr->get_next());
								deletedelete(curr);
							}
						     };
	public:		Lista();
			~Lista(){remove_Node(first);}
			Nodo* add_node(Nodo*);
			Iterator* create_iterator();
};			

class Iterator{
	private:	Lista* lista;
			Nodo* current;
	public:		Iterator(Lista* lista);
			~Iterator();
			Nodo* get_current();
			Nodo* move_next();
			Nodo* move_first();
			bool is_done;
};

class Connessione:public Nodo {
	private:	int conn_id;
	public:		Connessione(int conn_id);
			~Connessione();
			bool invia(char* msg);//send()
			char* ricevi();//recv()
};

class Conn_Client:public Connessione {
	public:		Conn_Client(int sock_id,char* Address);
			~Conn_Client();//cippa
};

class Conn_Server:public Connessione {
	public:		Conn_Server(int conn_id,char* Address);
			~Conn_Server();//close()
};


/*Iterator* it;
for( it=lista->create_Iterator();elemento = it->move_next(); elemento->do_something())*/
