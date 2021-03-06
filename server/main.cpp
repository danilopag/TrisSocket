// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include "stdafx.h"

/*
Simple UDP Server
*/
void stampamatrice();
void insertm(char mossa);
void insertmia(char mossa);
int controllav(char mossa);
int gestionetris(char message, int n);

#include<stdio.h>
#include<iostream>
using namespace std;
#include<winsock.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 2000   //The port on which to listen for incoming data
char matrice [3][3]={{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};
char mosse[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};

int main(){
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	char message[BUFLEN];
	int mfatta=1;//valore per vedere se la mosssa � stata fatta
	int i=0;
	int j=0;
	WSADATA wsa;

	slen = sizeof(si_other);

	//Initialise winsock

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//keep listening for data
		do{
		system("cls");
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				matrice[i][j]=' ';
			}
		}
		for(j=0;j<9;j++){
				mosse[j]=' ';
			}
		i=0;
		mfatta=1;
		printf("----ISTRUZIONI----\n");		
		printf("Questa e' la matrice del gioco \n inserisci il numero che corrisponde alla casella che vuoi crociare\n");
		printf("Il tuo simbolo e' la O\n");
		printf(" 1 | 2 | 3 \n");
		printf("-----------\n");
		printf(" 4 | 5 | 6\n");
		printf("-----------\n");
		printf(" 7 | 8 | 9 \n");
		printf("------------------\n\n");
	while (1)
	{	
		if(mfatta!=0){
		stampamatrice();
		printf("Aspetta la mossa dell'avversario...\n");
		fflush(stdin);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		if(buf[0]=='a'){
		system("cls");
		printf("Hai VINTOOOOO!!!!\n"); 
		stampamatrice();	
		break;
		}
		if(i==8){
		system("cls");
		printf("PAREGGIO!!!!\n"); 
		mosse[i]=buf[0];
		insertm(buf[0]);	
		message[0]='b';
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR){
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		} 
		stampamatrice();	
		break;			
		}
		mosse[i]=buf[0];
		insertm(buf[0]);
		i++;
		if(controllav(buf[0])==1){ 
		system("cls");
		printf("Hai PERSO!!!!\n");
		message[0]='a';
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR){
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		} 
		stampamatrice();	
		break;
		return 0;}
		}//chiusura della condizionde della mossa fatta
		stampamatrice();
		printf("Inserisci la mossa: \n");
		cin>>message;
		if(mfatta=gestionetris(message[0],i)==1){
			mosse[i]=message[0];
		    insertmia(message[0]);
		    i++;
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR){
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
			}
		}
		fflush(stdin);
		system("cls");
	}
		fflush(stdin);
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR){
			exit(EXIT_FAILURE);
		}
		if(buf[0]!='S'&&buf[0]!='s'){
			printf("l'avversario non ha voluto rigiocare!\n");
			closesocket(s);
	        WSACleanup();
	        system("PAUSE");
			return 0;
		}
		do{
		printf("vuoi rigiocare?(S/N)\n");
		fflush(stdin);
		cin>>message;		
		}while(!(message[0]=='S'||message[0]=='s'||message[0]=='N'||message[0]=='n'));
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		memset(buf, '\0', BUFLEN);
		if(message[0]!='S'&&message[0]!='s'){
			closesocket(s);
	        WSACleanup();
	        system("PAUSE");
			return 0;
		}
		}while(message[0]=='S'||message[0]=='s');
	closesocket(s);
	WSACleanup();
    system("PAUSE");
	return 0;
}

void stampamatrice(){
	printf(" %c | %c | %c \n",matrice[0][0],matrice[0][1],matrice[0][2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n",matrice[1][0],matrice[1][1],matrice[1][2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n",matrice[2][0],matrice[2][1],matrice[2][2]);
}

void insertm(char mossa){
	switch(mossa){
		case '1':	matrice[2][0]='X';
					break;
		case '2':	matrice[2][1]='X';
					break;
		case '3':	matrice[2][2]='X';
					break;		
		case '4':	matrice[1][0]='X';
					break;
		case '5':	matrice[1][1]='X';
					break;
		case '6':	matrice[1][2]='X';
					break;
		case '7':	matrice[0][0]='X';
					break;
		case '8':	matrice[0][1]='X';
					break;
		case '9':	matrice[0][2]='X';
					break;								
	}
}

void insertmia(char mossa){
	switch(mossa){
		case '1':	matrice[2][0]='O';
					break;
		case '2':	matrice[2][1]='O';
					break;
		case '3':	matrice[2][2]='O';
					break;		
		case '4':	matrice[1][0]='O';
					break;
		case '5':	matrice[1][1]='O';
					break;
		case '6':	matrice[1][2]='O';
					break;
		case '7':	matrice[0][0]='O';
					break;
		case '8':	matrice[0][1]='O';
					break;
		case '9':	matrice[0][2]='O';
					break;								
	}
}

int controllav(char mossa){
	switch(mossa){
		case '1':	if((matrice[2][1]=='X'&&matrice[2][2]=='X')||(matrice[0][0]=='X'&&matrice[1][0]=='X')||(matrice[1][1]=='X'&&matrice[0][2]=='X')){ return 1;}
					return 0;
					break;
		case '2':	if((matrice[2][0]=='X'&&matrice[2][2]=='X')||(matrice[1][1]=='X'&&matrice[0][1]=='X')){ return 1;}
					return 0;
					break;
		case '3':	if((matrice[2][0]=='X'&&matrice[2][1]=='X')||(matrice[0][2]=='X'&&matrice[1][2]=='X')||(matrice[1][1]=='X'&&matrice[0][0]=='X')){ return 1;}
					return 0;
					break;		
		case '4':	if((matrice[2][0]=='X'&&matrice[0][0]=='X')||(matrice[1][1]=='X'&&matrice[1][2]=='X')){ return 1;}
					return 0;
					break;
		case '5':	if((matrice[2][0]=='X'&&matrice[0][2]=='X')||(matrice[0][0]=='X'&&matrice[2][2]=='X')||(matrice[2][1]=='X'&&matrice[0][1]=='X')||(matrice[1][0]=='X'&&matrice[1][2]=='X')){ return 1;}
					return 0;
					break;
		case '6':	if((matrice[2][2]=='X'&&matrice[0][2]=='X')||(matrice[1][1]=='X'&&matrice[1][0]=='X')){ return 1;}
					return 0;
					break;
		case '7':	if((matrice[0][1]=='X'&&matrice[0][2]=='X')||(matrice[1][0]=='X'&&matrice[2][0]=='X')||(matrice[1][1]=='X'&&matrice[2][2]=='X')){ return 1;}
					return 0;
					break;
		case '8':	if((matrice[0][0]=='X'&&matrice[0][2]=='X')||(matrice[1][1]=='X'&&matrice[2][1]=='X')){ return 1;}
					return 0;
					break;
		case '9':	if((matrice[0][1]=='X'&&matrice[0][0]=='X')||(matrice[2][2]=='X'&&matrice[1][2]=='X')||(matrice[1][1]=='X'&&matrice[2][0]=='X')){ return 1;}
					return 0;
					break;								
	}
}

int gestionetris(char message, int n){
	int i;
	for(i=0;i<n;i++){
		if(mosse[i]==message||(message<'1'||message>'9')){
			printf("Inserisci una posizione corretta\n");
			system("cls");
			return 0;
		}
	}
	return 1;
}

