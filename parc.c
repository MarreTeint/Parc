#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

//constante pour le nombre de threads clients max, la journée dans le parc, nb attractions
#define MAX_CLIENTS 10
#define MAX_ATTRACTIONS 10

pthread_mutex_t mutexEntree = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int numero;
    int satisfaction;
    pthread_t thread;
    int argent; //<- vraiment utile ?
} client;

typedef struct {
    int numero;
    bool libre;
    int capacite;
    int duree;
    sem_t semaphore;
} attraction;

attraction attractions[MAX_ATTRACTIONS];
int caisse = 0;
int caisseJour=0;
int nbClients = 0;

void affichage(){
    //TODO Pour chaque attraction & l'accueil, recuperer le nombre de personne en file d'attente + le nombre de personne dedans
    //num=attractions[idat].capacite-file; ==> Pour affichage
    //Pour la file, faire un compteur de personne en file
    system("clear");
    printf("_________________________________________________\n|                                               |\n");
    /*for(int i=0;i<MAX_ATTRACTIONS;i++){
      
    }*/
    printf("|                                               |\n_________________________________________________\n");
    printf("Nombre de client(s) dans le parc : %d \n", nbClients);
    

}

void *process_client(void *arg) {
    pthread_mutex_lock(&mutexEntree);
    //printf("Je suis le client n° %d et je rentre dans le parc\n", ((client*)arg)->numero);
    caisseJour = caisseJour+30;
    nbClients++;
    //printf("J'ai payé les 30 euros !\n");
    pthread_mutex_unlock(&mutexEntree);
    while (true){
        affichage();
        int choix = rand()%3;
        switch (choix)
        {
        case 0:
            //printf("Je suis le client n° %d et je sors du parc\n", ((client*)arg)->numero);
            nbClients--;
            pthread_exit(NULL);
            break;
        
        case 1: ;
            int attractNum = rand()%MAX_ATTRACTIONS;
            //printf("Je suis le client n° %d et je vais à l'attraction n° %d\n", ((client*)arg)->numero, attractNum);
            
            process_attraction(attractNum);
            
            //printf("Je suis le client n° %d et je sors à l'attraction n° %d\n", ((client*)arg)->numero, attractNum);
            //Passe en suite dans l'allée à la fin de son attraction
        
        case 2: ;
            int howLong = rand()%5;
            // printf("Je suis le client n° %d et je reste dans l'allée %d secondes\n", ((client*)arg)->numero, howLong);
            sleep(howLong);
            break;
        }
            
    }
}


// possiblement faire un seul sleep pour touts les clients
void process_attraction(int idat) {
    
    int file;
    sem_getvalue(&attractions[idat].semaphore, &file);

    //int num;
    
    //printf("Je rentre\n");
    if (attractions[idat].libre){
        sleep(rand()%7);
    }
    else{
        sem_wait(&attractions[idat].semaphore);
        
        //printf("Attraction n° %d en cours\n", idat);

        sleep(attractions[idat].duree);

        
        //printf("Attraction n° %d finie\n", idat);
        sem_post(&attractions[idat].semaphore);
    }
}






int main(int argc, char const *argv[]) {
    char prochainJour = 'n';
    
    srand(time(NULL));
    int nbClients = rand()%MAX_CLIENTS+10;
    
    int nbJour = 0;
    client clients[nbClients];
    time_t fin; // en seconde
    time_t debut;
    fin = time(NULL) + 180;
    debut = time(NULL);

    //initialisation des attractions et des clients 
    for (int i = 0; i < MAX_ATTRACTIONS; i++) {
        attractions[i].numero = i;
        if (i%2 == 0) {
            attractions[i].libre = true;
        }
        attractions[i].libre = false;
        attractions[i].capacite = rand()%10+1;
        attractions[i].duree = rand()%10+1;
        sem_init(&attractions[i].semaphore, 0, attractions[i].capacite);


    }
    //pas oublier de vider les actractions à la fin du jour

    


    do {
      nbJour++;
      caisseJour = 0;
      for (int i = 0; i < nbClients; i++) {
      // sleep(0.01);
        clients[i].numero = i;
        clients[i].satisfaction = 100;
        clients[i].argent = 100;
        pthread_create(&clients[i].thread, NULL, &process_client, &clients[i]);
      }
      
      
      /*while ((fin - debut) > 0) {
        debut = time(NULL);
        if (debut%10 == 0) {
          affichage();
        }
      }*/
      
      
        for (int i = 0; i < nbClients; i++) {
        pthread_join(clients[i].thread, NULL);
    }
    printf("Nombre de clients aujourd'hui: %d\n", nbClients);

        
        
        printf("Nombre de clients aujourd'hui: %d\n", nbClients);

        
        caisse= caisse+caisseJour;
        printf("Fin journée n° %d, Chiffre d'affaires du jour : %d €\n", nbJour, caisseJour);
        
        printf("Jour suivant ? (o/n) \n");
        scanf(" %c", &prochainJour);
    } while  (prochainJour == 'o');  
    // en fonction du taux de satisfaction => si inf à 50 : 1/2 de revenir etc
    //TODO déterminer les horraires du parc (pas de 24h/24)


    //Fin de la journée
    
    

    printf("Caisse du parc au bout de %d jour(s): %d € \n",nbJour, caisse);
    return 0;
}


