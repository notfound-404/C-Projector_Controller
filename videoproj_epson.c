/****************************************************/
/* Programme  : EPSON.exe                           */
/* Objectif   : Controle videoprojecteur EPSON      */
/*            : Envoie de commandes via Socket      */
/* Auteur     : P. Boris                            */
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
#define PORT_SERV 3629
#define SOCK_ERREUR -1
// Definition de l'adresse IP de EP1
#define IP_EP1 "10.190.11.32"
#define IP_EP2 "xx.xx.xx.xx"
#define IP_EP3 "yy.yy.yy.yy"

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

  // Correspond au message  "ESC/VP.net" pour établissement de connexion
  char msgConnect[]={0x45, 0x53, 0x43, 0x2f, 0x56, 0x50, 0x2e, 0x6e, 0x65, 0x74, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00 };

  char msgCommandePWROFF[]="PWR OFF\x0d";
  char msgCommandePWRON[]="PWR ON\x0d";

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
  serv_addr.sin_addr.s_addr = inet_addr(IP_EP1);
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
    printf("|   1) POWER ON EP1       |\n");
    printf("|   2) POWER OFF EP1      |\n");
    printf("|                         |\n");
    printf("|   0) QUIT               |\n");
    printf("+-------------------------+\n");
    printf("| @IP EP1 : %s  |\n",IP_EP1);
    printf("+-------------------------+\n");

    printf("\nChoix : ");scanf("%d", &choix);

    switch(choix){

      case 0 :
        printf("[!] Fermeture normale du programme... \n");
        vrai=0;

      case 1 :                           
        /****************************[Attachement & Connexion de la socket *******************/
        bind(sockCommandePWRON, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr_in));
        connect(sockCommandePWRON, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
        printf("[+] Connexion au videoprojecteur EPSON \n");
        /****************************[ Premier message / Message de connection ]***************/
        /* Envoi du message msg au serveur*/
        len = sizeof(msgConnect);
        send(sockCommandePWRON, msgConnect, len, 0);
        sleep(nbSleep/2);
        /*****************************[ Deuxieme message / Envoi de la commande ]**************/
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
        printf("[+] Connexion au videoprojecteur EPSON \n");
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
