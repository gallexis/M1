#include <dirent.h>
#define SIZE_LINE 256

/* Serveur envoie liste fichiers au client,
   fichier par fichier, et utilise la chaine de
   caractère: "##no_more_file##" pour indiquer la fin de 
   l'envoie. (un fichier unix ne peut commencer par #,
   donc il n'y a pas de risque de conflit)
 */
void send_list_of_files(int sock,char *path){
    
    char buff[SIZE_LINE];
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir (path)) != NULL) {
        
        /* parcourir chaque fichier du répertoire */
        while ((ent = readdir (dir)) != NULL)
        {
            if (send(sock,ent->d_name,strlen(ent->d_name)+1,0) == -1) {
                perror("send"); exit(1);
            }
        }
        closedir (dir);
        
    } else {
        
        /* Erreur lors de l'ouverture du répertoire */
        perror ("opendir: ");
        return EXIT_FAILURE;
    }
    
    buff = "##no_more_file##"; // ou utiliser strcpy si ca marche pas
    
    if (send(sock,buff,strlen(buff)+1,0) == -1) {
        perror("send"); exit(1);
    }
}


/* client recoit liste des fichiers du serveur */
void receive_list_of_files(int sock){
    char buff[SIZE_LINE];
    
    while(recv(sock, buff, sizeof(buffer),0) > 0){
        
        if(strcmp(buff,"##no_more_file##") == 0)
            break
            
        printf("%s",buff);
        // memset buffer \0  ??
    }
}

/* Client ou serveur attend la taille du fichier */
int get_size(int sock){
    char buff[sizeof(int)];
    
    recv(sock,buff,sizeof(buff),0);
    return atoi(buff)
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
    if (send(sock,size+'0',strlen(size+'0')+1,0) == -1) {
        perror("send"); exit(1);
    }
    
    fclose(fp);
}



/*
    buff est in tableau de int dans send_data et get_data
    car on manipule des données binaires
 */

/* le client ou serveur envoie les données du fichier */
void send_data(char *path,int sock){
    int buff[SIZE_LINE];
    FILE *fp;
    
    if((fp = fopen(path,"rb"))==NULL)
    {
        perror("Erreur\n");
        exit(1);
    }
    
    while (fread(buff,SIZE_LINE,sizeof(int),fp) >= 0 )
    {
        if (send(sock,buff,strlen(buff)+1,0) == -1) {
            perror("send"); exit(1);
        }
    }
}

/* le client ou serveur recoit les données du fichier */
void get_data(FILE *fp,int sock,int size_of_file){

    int data_received;
    int tmp;
    int buff[SIZE_LINE];
    
    data_received = 0;
    tmp = 0;
    
    while (data_received < size_of_file) {
        tmp = recv(sock, buff, sizeof(buffer),0);
        if(tmp == 0){
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        
        fwrite(buff,sizeof(int),tmp,fp);
        
        data_received += tmp;
    }
    fclose(fp);
}