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

class SocketUDP:public Socket{
	private:	int ret_code;
	public:		
			SocketUDP();
			~SocketUDP();
			bool invia(char* msg,Address* dest){
								
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

SocketUDP()::SocketUDP():Socket(SOCK_DGRAM){  };

class ClientUDP:public SocketUDP{
	private:
	public:		ClientUDP();
			~ClientUDP();

};

class ServerUDP:public SocketUDP{
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
					  
				      };
};
class ServerTCP:SocketTCP{
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
	public:		Nodo(Nodo* next){
					_next= next;
				}
			~Nodo();		
			void set_next(Nodo* next){
				_next= next;
			}
			Nodo* get_next(){
				return _next;
			}
};

class Iterator;

class Lista{
	private:	Nodo* first;
			void remove_Node(Nodo* curr) {
							if (curr){
								remove_Node(curr->get_next());
								deletedelete(curr);
							}
						    }
	public:		Lista();
			~Lista(){
				remove_Node(first);
			}

			Nodo* add_node(Nodo* nuovo){
				Nodo* ultimo;

				if(ultimo=ultimo_nodo())
					ultimo->set_next(nuovo);
				else
					_first= nuovo;
			}

			Iterator* create_iterator()
			{
				return new Iterator(this);
			}

			Nodo* get_first(){
				return _first;
			}

	private:
			Nodo* ultimo_nodo(){ //Ritorna il puntatore all'ultimo Nodo della lista. NULL se la lista non possiede nodi.

				if(!first)
					return NULL;

				Nodo* ret= _first;

				while(ret->get_next())
					ret= ret->get_next();

				return ret;
			}
};			

class Iterator{
	private:	Lista* _lista;
				Nodo* _current;
	public:		

			bool is_done;

			Iterator(Lista* lista){
					_lista= lista;
					_current= lista->get_first();

					if(_current->get_next())
						is_done= false;
					else
						is_done= true;
			}

			~Iterator();

			Nodo* get_current(){
				return _current;
			}

			Nodo* move_next(){

				if(_current->get_next())
				{
					_current= _current->get_next();
					is_done= false;
				}
				else
					is_done= true;
			}

			Nodo* move_first(){
				_current= _lista->get_first();
				is_done= false;
			}
};

class Connessione:public Nodo {
	private:	int conn_id;
	public:		Connessione(int conn_id);
			~Connessione();
			bool invia(char* msg);//send()
			char* ricevi();//recv()
};

class Connessione::bool invia(char* msg)
				{
					ret_code=send(this, msg, strlen(msg), 0);
					if(ret_code<0) return false;
					else return true;
				}
class Connessione::char*ricevi()
			{
				char buffer[maxbuffer+1];
					  ret_code = recv(connessione, buffer, maxbuffer, 0); 
					  if(ret_code <= 0) return null;
					  buffer[ret_code]='\0';
					  return strdup(buffer);
			}

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
