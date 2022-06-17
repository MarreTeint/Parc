#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

//constante pour le nombre de threads clients max, la journée dans le parc, nb attractions
#define MAX_CLIENTS 1000
#define MAX_ATTRACTIONS 10
//#define DUREE_JOURNEE 60

//gestion du temps
//time_t fin; // en seconde
//time_t debut;


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
    int clientIn;
    int file;
    sem_t semaphore;
} attraction;

attraction attractions[MAX_ATTRACTIONS];
int caisse = 0;
int caisseJour=0;
int nbClients;
int nbClientsIn = 0;
int nbClientsOut = 0;
int nbClientsInAllee = 0;
bool start = false;


void affichage(){

//    system("clear");
    printf("Client(s) dans le parc : %d\nFile d'attente d'entrée : %d\nNombre de clients dans les alléees : %d\n", nbClientsIn, nbClients-nbClientsIn-nbClientsOut, nbClientsInAllee);
    for(int i=0; i<MAX_ATTRACTIONS; i++){
        if (!attractions[i].libre){
            printf("Attraction n°%d : %d clients / %d clients \nFile d'attente de l'attraction : %d clients\n",attractions[i].numero, attractions[i].clientIn,attractions[i].capacite, attractions[i].file);
        }
        else{
            printf("Attraction n°%d : %d clients \n",attractions[i].numero, attractions[i].clientIn);
        }
    }
    printf("-------------------------------\n");


}

void process_attraction(int idat, client* c) {
    //printf("Je rentre\n");
    if (attractions[idat].libre){
        attractions[idat].clientIn++;
        sleep(rand()%7);
        attractions[idat].clientIn--;
    }
    else{
        attractions[idat].file++;
        time_t debutAttente = time(NULL);
        sem_wait(&attractions[idat].semaphore);
//        sleep(11);
        time_t finAttente = time(NULL);
        if (finAttente-debutAttente > 10){
//            printf("Client n°%d : %d secondes d'attente pour rentrer dans l'attraction\n", c->numero, finAttente-debutAttente);
            c->satisfaction = c->satisfaction - 10;
            printf("Client n°%d : %d satisfaction\n", c->numero, c->satisfaction);
        }
        attractions[idat].file--;
        attractions[idat].clientIn++;
        //printf("Attraction n° %d en cours\n", idat);

        sleep(attractions[idat].duree);

        //printf("Attraction n° %d finie\n", idat);
        sem_post(&attractions[idat].semaphore);

        attractions[idat].clientIn--;
    }
}


void *process_client(void *arg) {
    pthread_mutex_lock(&mutexEntree);
    //printf("Je suis le client n° %d et je rentre dans le parc\n", ((client*)arg)->numero);
    start = true;
    caisseJour = caisseJour+30;
    nbClientsIn++;

    //printf("J'ai payé les 30 euros !\n");
    pthread_mutex_unlock(&mutexEntree);

    while (true){
        int choix = rand()%3;
        switch (choix)
        {
            case 0:
                //printf("Je suis le client n° %d et je sors du parc\n", ((client*)arg)->numero);
                nbClientsIn--;
                nbClientsOut++;
                pthread_exit(NULL);
                break;

            case 1: ;
                int attractNum = rand()%MAX_ATTRACTIONS;
                //printf("Je suis le client n° %d et je vais à l'attraction n° %d\n", ((client*)arg)->numero, attractNum);

                process_attraction(attractNum, (client*)arg);

                //printf("Je suis le client n° %d et je sors à l'attraction n° %d\n", ((client*)arg)->numero, attractNum);
                //Passe en suite dans l'allée à la fin de son attraction

            case 2: ;
                nbClientsInAllee++;
                int howLong = rand()%5;
                // printf("Je suis le client n° %d et je reste dans l'allée %d secondes\n", ((client*)arg)->numero, howLong);
                sleep(howLong);
                nbClientsInAllee--;
                break;
        }

    }
}

int main(int argc, char const *argv[]) {
    char prochainJour = 'n';

    srand(time(NULL));
    nbClients = rand()%MAX_CLIENTS+10;

    int nbJour = 0;
    client clients[nbClients];

//    fin = time(NULL) + 180;
//    debut = time(NULL);

    //initialisation des attractions et des clients
    for (int i = 0; i < MAX_ATTRACTIONS; i++) {
        attractions[i].numero = i;
        if (i%2 == 0) {
            attractions[i].libre = true;
        }
        attractions[i].libre = rand()%2;
        attractions[i].capacite = rand()%10+1;
        attractions[i].duree = rand()%10+1;
        attractions[i].clientIn = 0;
        sem_init(&attractions[i].semaphore, 0, attractions[i].capacite);


    }
    //initialisation des clients
    for (int i = 0; i < nbClients; i++) {
        // sleep(0.01);
        clients[i].numero = i;
        clients[i].satisfaction = 100;
        clients[i].argent = 100;
    }

    do {
        nbJour++;
        caisseJour = 0;
        //seul les clients satisfaits reviennent, s'ils ont une satisfaction < 60, ils ne reviendront pas
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].satisfaction > 90) {
                pthread_create(&clients[i].thread, NULL, &process_client, &clients[i]);
            } else {
                printf("Client n°%d ne reviendra pas : satisfaction : %d\n", clients[i].numero, clients[i].satisfaction);
            }
        }
        //fonction d'affichage
        while (nbClientsIn > 0 || !start) {
          affichage();
          sleep(1);
          debut = time(NULL);
        }

        for (int i = 0; i < nbClients; i++) {
            pthread_join(clients[i].thread, NULL);
        }
        affichage();
        printf("Nombre de clients aujourd'hui: %d\n", nbClients);

        caisse= caisse+caisseJour;
        nbClientsIn = 0;
        nbClientsOut = 0;
        nbClientsInAllee = 0;
        printf("Fin journée n° %d, Chiffre d'affaires du jour : %d €\n", nbJour, caisseJour);

        printf("Jour suivant ? (o/n) \n");
        scanf(" %c", &prochainJour);
    } while  (prochainJour == 'o');

    printf("Caisse du parc au bout de %d jour(s): %d € \n",nbJour, caisse);
    return 0;
}

