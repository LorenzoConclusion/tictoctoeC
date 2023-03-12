/* Programme client : envoie un message à un serveur */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include<unistd.h>
int main()
{
int sock_vers_serv ; /* sock_vers_serv utilisée pour envoyer au serveur un message */
struct hostent *num_ip_serv ; /* pointeur sur une structure donnant : nom, @ IP; ... du serveur */
struct sockaddr_in adr_serv ; /* adresse socket serveur*/
int lg_adr_serv; /* longueur de l’adr. la socket serveur*/
struct sockaddr_in adr_client ; /* adresse socket distante*/
int lg_adr_client; /* longueur de l’adresse de la socket client*/
char message[256] ;
/* création de la socket vers le serveur */
if ((sock_vers_serv = socket(AF_INET, SOCK_STREAM,0))==-1)
{ printf("echec création de la socket vers le serveur\n");
return -1 ;
}
/* la socket est attachée à une adresse IP et un numéro de port (IPPORT_USERRESERVED+1) */
bzero((char *)&adr_client, sizeof(adr_serv)) ;
adr_client.sin_port = IPPORT_USERRESERVED+1 ;
adr_client.sin_addr.s_addr = INADDR_ANY ;
adr_client.sin_family =AF_INET ;
bind (sock_vers_serv, (struct sockaddr*)&adr_client, sizeof(adr_client)) ;
/* connexion au serveur*/
bzero((char *)&adr_serv, sizeof(adr_serv)) ;
if ((num_ip_serv=gethostbyname("127.0.0.1"))==NULL)
{printf(" nom de la machine serveur inconnu \n");
return -3;
}
else
{bcopy(num_ip_serv->h_addr, &adr_serv.sin_addr,num_ip_serv->h_length) ;
}
adr_serv.sin_port = IPPORT_USERRESERVED ;
adr_serv.sin_family =AF_INET ;
if (connect (sock_vers_serv, (struct sockaddr*)&adr_serv, sizeof(adr_serv))==-1)
{
printf(" echec connexion \n") ;
return -2 ;
}
printf("Donner votre message au serveur :");
scanf("%s",message);


/* émission d’un message sur la socket au moyen de la primitive write. IL existe une autre primitive d’émission « send », celle-ci utilise un paramètre supplémentaire « in t flag » (en dernier param.) permettant d’envoyer des données de façon express (flag=MSG_OOB), ... */
write (sock_vers_serv,message ,sizeof(message) );
read(sock_vers_serv,message,sizeof(message)) ;
printf("Message recuServeur : %s\n", message);
}