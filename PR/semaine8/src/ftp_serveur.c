#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#define SIZEMSG 255
#define TAILLE_NOM_FICHIER 30
#define TAILLE_NOM_REPERTOIRE 10
char repertoire[TAILLE_NOM_REPERTOIRE];
int PORTSERV;
struct sockaddr_in sin;
int sc ;
int scom;
struct sockaddr_in exp;
int fromlen;

struct msg{
    char cmd[8];
    char fichier[TAILLE_NOM_FICHIER];
} mymsg;


void send_list_of_files(char *path){
    
    char buff[SIZEMSG];
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path)) != NULL) {
        /* parcourir chaque fichier du répertoire */
        while ((ent = readdir (dir)) != NULL)
        {
            
            if (send(scom,ent->d_name,SIZEMSG,0) == -1) {
                perror("send"); exit(1);
            }
        }
        closedir (dir);
        
    } else {
        
        /* Erreur lors de l'ouverture du répertoire */
        perror ("opendir: ");
        return EXIT_FAILURE;
    }
    
    strcpy(buff,"##no_more_file##");
    
    if (send(scom,buff,strlen(buff)+1,0) == -1) {
        perror("send"); exit(1);
    }
}




/* Client ou serveur attend la taille du fichier */
int get_size(int sock){
    char buff[sizeof(int)];
    
    recv(scom,buff,sizeof(buff),0);
    return atoi(buff);
}

/* client ou serveur envoie la taille du fichier */
void send_size(char *path,int sock){
    int size;
    FILE *fp;
    
    if((fp = fopen(path,"rb"))==NULL)
    {
        perror("Erreur\n");
        exit(1);
    }
    
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    
    /* size+'0' est pour convertir le int en char */
    if (send(scom,size+'0',strlen(size+'0')+1,0) == -1) {
        perror("send"); exit(1);
    }
    
    fclose(fp);
}

void SendFile(char *name)
{
    char *nom_complet;
    nom_complet=malloc(TAILLE_NOM_FICHIER+TAILLE_NOM_REPERTOIRE+1);
    strcpy(nom_complet,repertoire);
    strcat(nom_complet,name);
    char buffer[SIZEMSG];
    FILE *ptr;
    if((ptr = fopen(nom_complet,"rb"))==NULL)
    {
        perror("Erreur\n");
        exit(1);
    }
    while (fread(buffer,sizeof(buffer),1,ptr)>0) {
        if (write(scom,buffer,SIZEMSG) == -1) {
            perror("write"); exit(1);
        }
    }
}

void ReceiveFile(char *name,int taille)
{
    char *nom_complet;
    int taille_fichier=taille;
    int i;
    nom_complet=malloc(TAILLE_NOM_FICHIER+TAILLE_NOM_REPERTOIRE+1);
    strcpy(nom_complet,repertoire);
    strcat(nom_complet,name);
    char buffer[SIZEMSG];
    FILE *ptr;
    
    if((ptr = fopen(nom_complet,"wb"))==NULL)
    {
        perror("Erreur\n");
        exit(1);
    }
    while (taille_fichier>0) {
        if (i=read(scom,buffer, SIZEMSG) == -1) {
            perror("write");
            fclose(ptr);
            exit(1);
        }
        fwrite(buffer,sizeof(buffer),1,ptr);
        taille_fichier-=i;
    }
    fclose(ptr);
    
}


void *connection_handler(void *scom)
{
    int sock = *(int*)scom;
    char message[SIZEMSG];
    
    while(1)
    {
        if (read(scom,message, SIZEMSG) < 0) {
            perror("read"); exit(4);
        }
        printf("message %s \n", message);
        sscanf(message,"%s %s",&mymsg.cmd,&mymsg.fichier);
        
        if(strcmp(mymsg.cmd,OP_LIST)==0)
        {
            printf("Liste de fichier \n");
            send_list_of_files(repertoire);
        }
        if(strcmp(mymsg.cmd,OP_UPLOAD)==0)
        {
            printf("Upload de %s \n",mymsg.fichier);
        }
        if(strcmp(mymsg.cmd,OP_DOWNLOAD)==0)
        {
            printf("download de %s \n",mymsg.fichier);
        }
        if(strcmp(mymsg.cmd,OP_QUIT)==0)
        {
            printf("Quitter \n");
            close(scom);
            break;
        }
        
    }

}


int main(int argc, const char **argv)
{
    PORTSERV=atoi(argv[1]);
    strcpy(repertoire,argv[2]);
    char OP_LIST[4];
    char OP_UPLOAD[6];
    char OP_DOWNLOAD[8];
    char OP_QUIT[4];
    strcpy(OP_LIST,"LIST");
    strcpy(OP_UPLOAD,"UPLOAD");
    strcpy(OP_DOWNLOAD,"DOWNLOAD");
    strcpy(OP_QUIT,"QUIT");
    
    fromlen = sizeof exp;
    char message[SIZEMSG];
    pthread_t thread_id;
    pthread_attr_t attr;
    
    /*creation de la socket*/
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket"); exit(1);
    }
    memset((char *)&sin,0, sizeof(sin));
    
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORTSERV);
    sin.sin_family = AF_INET;
    
    if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind");
        exit(1);
    }
    
    listen(sc, 5);
    int fd1;
    int p;
    
    
    while( (scom = accept(sc, (struct sockaddr *)&exp, &fromlen)) )
    {
        puts("Connection accepted");
        
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        
        if( pthread_create( &thread_id , &attr ,  connection_handler , (void*) &scom) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
    
    
    shutdown(scom,2);
    
    close(sc);
    return (0);
}