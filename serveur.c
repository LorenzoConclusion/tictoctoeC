/* Programme serveur : affiche les messages qui proviennent des clients */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include<unistd.h>
int main()
{
int sock_ecoute, sock_dial ; /* sock_ecoute est utilis�e par le processus p�re
pour l'�coute des demandes de connexions, sock_dial est utilis�e par un fils pour
communiquer avec un client */
char tampon[256], ch[100] ; /* pour stocker un message re�u */
struct sockaddr_in adr_serv ; /* adresse socket serveur */
int lg_adr_serv; /* longueur de l'adresse de la socket serveur */
char nom_serv[30]; /* nom du serveur */
struct hostent *num_ip_serv; /* pointeur sur une structure donnant le nom,
l'adresse IP, etc, du serveur */
struct sockaddr_in adr_client ; /* adresse socket distante */
int lg_adr_client; /* longueur de l'adresse de la socket client */
/* cr�ation de la socket d'�coute */
sock_ecoute = socket(AF_INET, SOCK_STREAM,0) ;
/* la socket est attach�e � une adresse IP et un num�ro de port
(IPPORT_USERRESERVED, ne n�cessite pas un mode privil�gi�) */


//socket option 
int option=1;
setsockopt(sock_ecoute,SOL_SOCKET,SO_REUSEADDR,(void*)&option,sizeof(option));



bzero((char *)&adr_serv, sizeof(adr_serv)) ;
adr_serv.sin_port = IPPORT_USERRESERVED ;
adr_serv.sin_addr.s_addr = INADDR_ANY; /*pour accepter une adresse quelconque
(0.0.0.0) */
/* autre solution pour obtenir l'adresse IP de la machine locale : */
gethostname(nom_serv, (size_t) 30);
//printf("nom du serveur %s \n", nom_serv);
if ((num_ip_serv=gethostbyname("127.0.0.1"))==NULL)
{ printf(" �chec dans l'obtention de l'adresse IP du serveur \n");
return -1;
}
else
{
bcopy(num_ip_serv->h_addr,&adr_serv.sin_addr,num_ip_serv->h_length);} /* fin
solution */
adr_serv.sin_family =AF_INET ;
bind (sock_ecoute, (struct sockaddr*)&adr_serv, sizeof(adr_serv)) ; /* attachement
de la socket */
/* ouverture du service : cr�ation d'une file d'attente pour les demandes de
connexion d'une longueur max. �gale � 5*/
listen(sock_ecoute,5) ;
for (;;)/* boucle infinie traitant les diff�rentes des clients */
{
/* attente d'une demande de connexion */
lg_adr_client = sizeof(adr_client) ;
sock_dial=accept(sock_ecoute, (struct sockaddr *)&adr_client, &lg_adr_client) ;
/* cr�ation d'un processus fil */
if (fork() ==0)
{ /* fils */
close(sock_ecoute) ;
/* r�ception d�un message sur la socket au moyen de la primitive read. IL existe
une autre primitive de r�ception � recv �, celle-ci utilise un param�tre
suppl�mentaire � in t flag � (en dernier param.) permettant de consulter des
donn�es sans les pr�lever (flag=MSG_PEEK), ... */
read(sock_dial,tampon,sizeof(tampon)) ;
printf("Le serveur a recu : %s\n", tampon) ;
printf("Donner votre chaine :");
scanf("%s",ch);
write (sock_dial,ch ,sizeof(ch) );
return 0 ;

}
close(sock_dial) ;
} /* for */
}/*main*/
