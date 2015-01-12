/****************************************************/
/* Programme  : SMART.exe                           */
/* Objectif   : Controle videoprojecteur SMART      */
/*            : Envoie de commandes via Socket      */
/* Auteur     : P Boris                             */
/* Date       : 25/04/2013                          */
/* Plateforme : Unix - Win32/64                     */
/* Version    : v0 (Version initiale)               */
/****************************************************/


#if defined (WIN32)
  #include <winsock2.h>
#elif defined (linux)
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#endif

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/*DEFINE*/
#define MAX 400
#define PORT_CLIENT 40011
#define PORT_SERV 80
#define SOCK_ERREUR -1
// Definition de l'adresse IP de EP1
#define IP_SM1 "192.168.1.100"
#define IP_SM2 "xx.xx.xx.xx"
#define IP_SM3 "yy.yy.yy.yy"

int main(){

  #if defined (WIN32)   //Si OS = Windows
    int nbSleep = 3000; //sleep(milliseconde) sous WINDOWS
    WSADATA WSAData;
    int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
  #else
    int nbSleep = 3;    //sleep(seconde) sous UNIX
  #endif

  /*** Déclaration structures pour sockets ***/
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;

  int sockCommandePWRON, sockCommandePWROFF, len, choix=0, vrai=1;

  // Correspond au message pour établissement de connexion
char msgConnect[] = {
0x50, 0x4f, 0x53, 0x54, 0x20, 0x2f, 0x74, 0x67, 
0x69, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 
0x6c, 0x2e, 0x74, 0x67, 0x69, 0x20, 0x48, 0x54, 
0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0x0d, 0x0a, 
0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20, 0x31, 0x30, 
0x2e, 0x31, 0x39, 0x30, 0x2e, 0x31, 0x31, 0x2e, 
0x34, 0x30, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x6e, 
0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 
0x6b, 0x65, 0x65, 0x70, 0x2d, 0x61, 0x6c, 0x69, 
0x76, 0x65, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x74, 
0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 0x67, 
0x74, 0x68, 0x3a, 0x20, 0x34, 0x34, 0x33, 0x0d, 
0x0a, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2d, 0x43, 
0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x3a, 0x20, 
0x6d, 0x61, 0x78, 0x2d, 0x61, 0x67, 0x65, 0x3d, 
0x30, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 
0x74, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 
0x68, 0x74, 0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 
0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 
0x2f, 0x78, 0x68, 0x74, 0x6d, 0x6c, 0x2b, 0x78, 
0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 0x6c, 0x69, 
0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x78, 
0x6d, 0x6c, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x39, 
0x2c, 0x2a, 0x2f, 0x2a, 0x3b, 0x71, 0x3d, 0x30, 
0x2e, 0x38, 0x0d, 0x0a, 0x4f, 0x72, 0x69, 0x67, 
0x69, 0x6e, 0x3a, 0x20, 0x68, 0x74, 0x74, 0x70, 
0x3a, 0x2f, 0x2f, 0x31, 0x30, 0x2e, 0x31, 0x39, 
0x30, 0x2e, 0x31, 0x31, 0x2e, 0x34, 0x30, 0x0d, 
0x0a, 0x55, 0x73, 0x65, 0x72, 0x2d, 0x41, 0x67, 
0x65, 0x6e, 0x74, 0x3a, 0x20, 0x4d, 0x6f, 0x7a, 
0x69, 0x6c, 0x6c, 0x61, 0x2f, 0x35, 0x2e, 0x30, 
0x20, 0x28, 0x58, 0x31, 0x31, 0x3b, 0x20, 0x4c, 
0x69, 0x6e, 0x75, 0x78, 0x20, 0x78, 0x38, 0x36, 
0x5f, 0x36, 0x34, 0x29, 0x20, 0x41, 0x70, 0x70, 
0x6c, 0x65, 0x57, 0x65, 0x62, 0x4b, 0x69, 0x74, 
0x2f, 0x35, 0x33, 0x37, 0x2e, 0x33, 0x31, 0x20, 
0x28, 0x4b, 0x48, 0x54, 0x4d, 0x4c, 0x2c, 0x20, 
0x6c, 0x69, 0x6b, 0x65, 0x20, 0x47, 0x65, 0x63, 
0x6b, 0x6f, 0x29, 0x20, 0x43, 0x68, 0x72, 0x6f, 
0x6d, 0x65, 0x2f, 0x32, 0x36, 0x2e, 0x30, 0x2e, 
0x31, 0x34, 0x31, 0x30, 0x2e, 0x36, 0x33, 0x20, 
0x53, 0x61, 0x66, 0x61, 0x72, 0x69, 0x2f, 0x35, 
0x33, 0x37, 0x2e, 0x33, 0x31, 0x0d, 0x0a, 0x43, 
0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x54, 
0x79, 0x70, 0x65, 0x3a, 0x20, 0x61, 0x70, 0x70, 
0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 
0x2f, 0x78, 0x2d, 0x77, 0x77, 0x77, 0x2d, 0x66, 
0x6f, 0x72, 0x6d, 0x2d, 0x75, 0x72, 0x6c, 0x65, 
0x6e, 0x63, 0x6f, 0x64, 0x65, 0x64, 0x0d, 0x0a, 
0x52, 0x65, 0x66, 0x65, 0x72, 0x65, 0x72, 0x3a, 
0x20, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 
0x31, 0x30, 0x2e, 0x31, 0x39, 0x30, 0x2e, 0x31, 
0x31, 0x2e, 0x34, 0x30, 0x2f, 0x63, 0x6f, 0x6e, 
0x74, 0x72, 0x6f, 0x6c, 0x2e, 0x68, 0x74, 0x6d, 
0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 
0x2d, 0x45, 0x6e, 0x63, 0x6f, 0x64, 0x69, 0x6e, 
0x67, 0x3a, 0x20, 0x67, 0x7a, 0x69, 0x70, 0x2c, 
0x64, 0x65, 0x66, 0x6c, 0x61, 0x74, 0x65, 0x2c, 
0x73, 0x64, 0x63, 0x68, 0x0d, 0x0a, 0x41, 0x63, 
0x63, 0x65, 0x70, 0x74, 0x2d, 0x4c, 0x61, 0x6e, 
0x67, 0x75, 0x61, 0x67, 0x65, 0x3a, 0x20, 0x66, 
0x72, 0x2d, 0x46, 0x52, 0x2c, 0x66, 0x72, 0x3b, 
0x71, 0x3d, 0x30, 0x2e, 0x38, 0x2c, 0x65, 0x6e, 
0x2d, 0x55, 0x53, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 
0x36, 0x2c, 0x65, 0x6e, 0x3b, 0x71, 0x3d, 0x30, 
0x2e, 0x34, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 
0x70, 0x74, 0x2d, 0x43, 0x68, 0x61, 0x72, 0x73, 
0x65, 0x74, 0x3a, 0x20, 0x49, 0x53, 0x4f, 0x2d, 
0x38, 0x38, 0x35, 0x39, 0x2d, 0x31, 0x2c, 0x75, 
0x74, 0x66, 0x2d, 0x38, 0x3b, 0x71, 0x3d, 0x30, 
0x2e, 0x37, 0x2c, 0x2a, 0x3b, 0x71, 0x3d, 0x30, 
0x2e, 0x33, 0x0d, 0x0a, 0x0d, 0x0a, 0x50, 0x57, 
0x52, 0x53, 0x54, 0x41, 0x54, 0x45, 0x3d, 0x32, 
0x26, 0x53, 0x4f, 0x55, 0x52, 0x43, 0x45, 0x53, 
0x45, 0x4c, 0x3d, 0x32, 0x26, 0x70, 0x6f, 0x77, 
0x65, 0x72, 0x3d, 0x45, 0x53, 0x26, 0x53, 0x74, 
0x61, 0x6e, 0x64, 0x62, 0x79, 0x4d, 0x6f, 0x64, 
0x65, 0x3d, 0x45, 0x74, 0x65, 0x69, 0x6e, 0x64, 
0x72, 0x65, 0x26, 0x56, 0x6f, 0x6c, 0x75, 0x6d, 
0x65, 0x3d, 0x30, 0x26, 0x6d, 0x75, 0x74, 0x65, 
0x3d, 0x31, 0x26, 0x56, 0x6f, 0x6c, 0x43, 0x74, 
0x72, 0x6c, 0x3d, 0x30, 0x26, 0x43, 0x43, 0x53, 
0x3d, 0x31, 0x26, 0x43, 0x43, 0x4c, 0x61, 0x6e, 
0x67, 0x3d, 0x30, 0x26, 0x56, 0x44, 0x6d, 0x6f, 
0x64, 0x65, 0x3d, 0x30, 0x26, 0x42, 0x72, 0x69, 
0x67, 0x68, 0x74, 0x3d, 0x35, 0x30, 0x26, 0x43, 
0x6f, 0x6e, 0x74, 0x72, 0x61, 0x73, 0x74, 0x3d, 
0x35, 0x30, 0x26, 0x53, 0x61, 0x74, 0x75, 0x72, 
0x61, 0x74, 0x69, 0x6f, 0x6e, 0x3d, 0x35, 0x30, 
0x26, 0x54, 0x69, 0x6e, 0x74, 0x3d, 0x35, 0x30, 
0x26, 0x53, 0x68, 0x61, 0x72, 0x70, 0x3d, 0x31, 
0x36, 0x26, 0x57, 0x70, 0x65, 0x61, 0x6b, 0x69, 
0x6e, 0x67, 0x3d, 0x31, 0x30, 0x26, 0x44, 0x65, 
0x67, 0x61, 0x6d, 0x6d, 0x61, 0x3d, 0x32, 0x26, 
0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x5f, 0x52 };

char msgCommandePWRON[] = {
0x50, 0x4f, 0x53, 0x54, 0x20, 0x2f, 0x74, 0x67, 
0x69, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 
0x6c, 0x2e, 0x74, 0x67, 0x69, 0x20, 0x48, 0x54, 
0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0x0d, 0x0a, 
0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20, 0x31, 0x39, 
0x32, 0x2e, 0x31, 0x36, 0x38, 0x2e, 0x31, 0x2e, 
0x31, 0x30, 0x30, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 
0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 
0x20, 0x6b, 0x65, 0x65, 0x70, 0x2d, 0x61, 0x6c, 
0x69, 0x76, 0x65, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 
0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 
0x67, 0x74, 0x68, 0x3a, 0x20, 0x35, 0x33, 0x0d, 
0x0a, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2d, 0x43, 
0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x3a, 0x20, 
0x6d, 0x61, 0x78, 0x2d, 0x61, 0x67, 0x65, 0x3d, 
0x30, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 
0x74, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 
0x68, 0x74, 0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 
0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 
0x2f, 0x78, 0x68, 0x74, 0x6d, 0x6c, 0x2b, 0x78, 
0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 0x6c, 0x69, 
0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x78, 
0x6d, 0x6c, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x39, 
0x2c, 0x2a, 0x2f, 0x2a, 0x3b, 0x71, 0x3d, 0x30, 
0x2e, 0x38, 0x0d, 0x0a, 0x4f, 0x72, 0x69, 0x67, 
0x69, 0x6e, 0x3a, 0x20, 0x68, 0x74, 0x74, 0x70, 
0x3a, 0x2f, 0x2f, 0x31, 0x39, 0x32, 0x2e, 0x31, 
0x36, 0x38, 0x2e, 0x31, 0x2e, 0x31, 0x30, 0x30, 
0x0d, 0x0a, 0x55, 0x73, 0x65, 0x72, 0x2d, 0x41, 
0x67, 0x65, 0x6e, 0x74, 0x3a, 0x20, 0x4d, 0x6f, 
0x7a, 0x69, 0x6c, 0x6c, 0x61, 0x2f, 0x35, 0x2e, 
0x30, 0x20, 0x28, 0x58, 0x31, 0x31, 0x3b, 0x20, 
0x4c, 0x69, 0x6e, 0x75, 0x78, 0x20, 0x78, 0x38, 
0x36, 0x5f, 0x36, 0x34, 0x29, 0x20, 0x41, 0x70, 
0x70, 0x6c, 0x65, 0x57, 0x65, 0x62, 0x4b, 0x69, 
0x74, 0x2f, 0x35, 0x33, 0x37, 0x2e, 0x33, 0x31, 
0x20, 0x28, 0x4b, 0x48, 0x54, 0x4d, 0x4c, 0x2c, 
0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x47, 0x65, 
0x63, 0x6b, 0x6f, 0x29, 0x20, 0x43, 0x68, 0x72, 
0x6f, 0x6d, 0x65, 0x2f, 0x32, 0x36, 0x2e, 0x30, 
0x2e, 0x31, 0x34, 0x31, 0x30, 0x2e, 0x36, 0x33, 
0x20, 0x53, 0x61, 0x66, 0x61, 0x72, 0x69, 0x2f, 
0x35, 0x33, 0x37, 0x2e, 0x33, 0x31, 0x0d, 0x0a, 
0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 
0x54, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x61, 0x70, 
0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 
0x6e, 0x2f, 0x78, 0x2d, 0x77, 0x77, 0x77, 0x2d, 
0x66, 0x6f, 0x72, 0x6d, 0x2d, 0x75, 0x72, 0x6c, 
0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x64, 0x0d, 
0x0a, 0x52, 0x65, 0x66, 0x65, 0x72, 0x65, 0x72, 
0x3a, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 
0x2f, 0x31, 0x39, 0x32, 0x2e, 0x31, 0x36, 0x38, 
0x2e, 0x31, 0x2e, 0x31, 0x30, 0x30, 0x2f, 0x63, 
0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x2e, 0x68, 
0x74, 0x6d, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 
0x70, 0x74, 0x2d, 0x45, 0x6e, 0x63, 0x6f, 0x64, 
0x69, 0x6e, 0x67, 0x3a, 0x20, 0x67, 0x7a, 0x69, 
0x70, 0x2c, 0x64, 0x65, 0x66, 0x6c, 0x61, 0x74, 
0x65, 0x2c, 0x73, 0x64, 0x63, 0x68, 0x0d, 0x0a, 
0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x2d, 0x4c, 
0x61, 0x6e, 0x67, 0x75, 0x61, 0x67, 0x65, 0x3a, 
0x20, 0x66, 0x72, 0x2d, 0x46, 0x52, 0x2c, 0x66, 
0x72, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x38, 0x2c, 
0x65, 0x6e, 0x2d, 0x55, 0x53, 0x3b, 0x71, 0x3d, 
0x30, 0x2e, 0x36, 0x2c, 0x65, 0x6e, 0x3b, 0x71, 
0x3d, 0x30, 0x2e, 0x34, 0x0d, 0x0a, 0x41, 0x63, 
0x63, 0x65, 0x70, 0x74, 0x2d, 0x43, 0x68, 0x61, 
0x72, 0x73, 0x65, 0x74, 0x3a, 0x20, 0x49, 0x53, 
0x4f, 0x2d, 0x38, 0x38, 0x35, 0x39, 0x2d, 0x31, 
0x2c, 0x75, 0x74, 0x66, 0x2d, 0x38, 0x3b, 0x71, 
0x3d, 0x30, 0x2e, 0x37, 0x2c, 0x2a, 0x3b, 0x71, 
0x3d, 0x30, 0x2e, 0x33, 0x0d, 0x0a, 0x0d, 0x0a, 
0x50, 0x57, 0x52, 0x53, 0x54, 0x41, 0x54, 0x45, 
0x3d, 0x34, 0x26, 0x53, 0x4f, 0x55, 0x52, 0x43, 
0x45, 0x53, 0x45, 0x4c, 0x3d, 0x35, 0x26, 0x70, 
0x6f, 0x77, 0x65, 0x72, 0x3d, 0x49, 0x6e, 0x61, 
0x63, 0x74, 0x69, 0x66, 0x26, 0x50, 0x6f, 0x77, 
0x65, 0x72, 0x4f, 0x6e, 0x3d, 0x41, 0x6c, 0x69, 
0x6d, 0x2b, 0x4f, 0x55, 0x49
};

char msgCommandePWROFF[] = {
0x3d, 0x31, 0x30, 0x30, 0x26, 0x43, 0x6f, 0x6c, 
0x6f, 0x72, 0x5f, 0x47, 0x3d, 0x31, 0x30, 0x30, 
0x26, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x5f, 0x42, 
0x3d, 0x31, 0x30, 0x30, 0x26, 0x43, 0x6f, 0x6c, 
0x6f, 0x72, 0x5f, 0x43, 0x3d, 0x31, 0x30, 0x30, 
0x26, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x5f, 0x4d, 
0x3d, 0x31, 0x30, 0x30, 0x26, 0x43, 0x6f, 0x6c, 
0x6f, 0x72, 0x5f, 0x59, 0x3d, 0x31, 0x30, 0x30, 
0x26, 0x41, 0x75, 0x74, 0x6f, 0x53, 0x44, 0x3d, 
0x31, 0x26, 0x4c, 0x61, 0x6d, 0x70, 0x52, 0x3d, 
0x30, 0x26, 0x48, 0x69, 0x42, 0x72, 0x69, 0x67, 
0x68, 0x74, 0x3d, 0x30, 0x26, 0x41, 0x75, 0x74, 
0x6f, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x6f, 0x66, 
0x66, 0x3d, 0x31, 0x32, 0x30, 0x26, 0x5a, 0x6f, 
0x6f, 0x6d, 0x3d, 0x30, 0x26, 0x41, 0x73, 0x70, 
0x65, 0x63, 0x74, 0x52, 0x61, 0x74, 0x69, 0x6f, 
0x3d, 0x30, 0x26, 0x53, 0x74, 0x61, 0x72, 0x74, 
0x55, 0x70, 0x3d, 0x30, 0x26, 0x76, 0x69, 0x64, 
0x65, 0x6f, 0x6d, 0x75, 0x74, 0x65, 0x3d, 0x31, 
0x26, 0x48, 0x69, 0x46, 0x61, 0x6e, 0x3d, 0x31, 
0x26, 0x50, 0x72, 0x6f, 0x6a, 0x4d, 0x6f, 0x64, 
0x65, 0x3d, 0x31, 0x26, 0x53, 0x6f, 0x75, 0x72, 
0x63, 0x65, 0x3d, 0x32, 0x26, 0x56, 0x47, 0x41, 
0x31, 0x54, 0x61, 0x67, 0x3d, 0x26, 0x56, 0x47, 
0x41, 0x32, 0x54, 0x61, 0x67, 0x3d, 0x26, 0x53, 
0x56, 0x69, 0x64, 0x65, 0x6f, 0x54, 0x61, 0x67, 
0x3d, 0x26, 0x43, 0x6f, 0x6d, 0x70, 0x6f, 0x73, 
0x69, 0x74, 0x65, 0x54, 0x61, 0x67, 0x3d, 0x26, 
0x48, 0x44, 0x4d, 0x49, 0x54, 0x61, 0x67, 0x3d, 
0x26, 0x65, 0x6d, 0x65, 0x72, 0x67, 0x65, 0x6e, 
0x63, 0x79, 0x61, 0x6c, 0x65, 0x72, 0x74, 0x3d, 
0x31, 0x26, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 
0x74, 0x3d };


  /************** Informations sur le client ****************/
  sockCommandePWRON = socket(AF_INET, SOCK_STREAM,0);   //Socket Internet MODE Connecte (SOCK_STREAM)
  sockCommandePWROFF = socket(AF_INET, SOCK_STREAM,0);
  memset(&cli_addr, 0, sizeof(struct sockaddr_in));
  cli_addr.sin_family = AF_INET;
  cli_addr.sin_addr.s_addr = INADDR_ANY;                //on laisse le client choisir l'interface reseau
  cli_addr.sin_port = htons(0);                         //on laisse le client choisir le port pour eviter les problemes

  /************* Informations sur le serveur ***************/
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(IP_SM1);
  serv_addr.sin_port = htons(PORT_SERV);


  /***********  Creation d'un menu pour l'utilisateur *********/
  while (vrai){
    #if defined (WIN32)
      system("cls");
    #else
      system("clear");
    #endif

    printf("+---------[ MENU ]--------+\n");
    printf("|                         |\n");
    printf("|   1) POWER ON SMART1    |\n");
    printf("|   2) POWER OFF SMART1   |\n");
    printf("|                         |\n");
    printf("|   0) QUIT               |\n");
    printf("+-------------------------+\n");
    printf("|@IP SMART1: %s|\n",IP_SM1);
    printf("+-------------------------+\n");

    printf("\nChoix : ");scanf("%d", &choix);

    switch(choix){

      case 0 :
        printf("[!] Fermeture normale du programme... \n");
        vrai=0;
        break;

      case 1 :                           
        /****************************[Attachement & Connexion de la socket *******************/
        bind(sockCommandePWRON, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr_in));
        connect(sockCommandePWRON, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
        printf("[+] Connexion au videoprojecteur SMART \n");
        /*****************************[ Premier message / Envoi de la commande ]**************/
        /* Envoi du message msg au serveur*/
        len = sizeof(msgCommandePWRON);
        send(sockCommandePWRON, msgCommandePWRON, len, 0);
        printf("[+] Envoi de la commande effectuee : Le videoprojecteur va s'allumer\n\n"); 

        close(sockCommandePWRON);
        sleep(nbSleep);
        break;

      case 2 :
        /****************************[ Attachement & Connexion de la socket ]*******************/
        bind(sockCommandePWROFF, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr_in));                        
        connect(sockCommandePWROFF, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
        printf("[+] Connexion au videoprojecteur SMART \n");
        /****************************[ Premier message / Message de connection ]*****************/
        len = sizeof(msgConnect);
        send(sockCommandePWROFF, msgConnect, len, 0);
        printf("[+] Envoi du message de connexion\n");
        sleep(nbSleep/2);
        /*****************************[ Deuxieme message / Envoi de la commande ]*****************/
        len = sizeof(msgCommandePWROFF);
        send(sockCommandePWROFF, msgCommandePWROFF, len, 0);
        printf("[+] Envoi de la commande effectuee : Le videoprojecteur va s'eteindre\n\n");
    
        close(sockCommandePWROFF);
        sleep(nbSleep);                 
        break;

      default:  /* Si l'utilisateur choisit une option inconnue */
        printf("\n[!] Choix invalide ! \n\n");
        sleep(nbSleep);
    }
  }

  #if defined (WIN32)
    WSACleanup();
  #endif

  sleep(nbSleep/3);
}
