#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

//constante pour le nombre de threads clients max, la journée dans le parc, nb attractions
#define MAX_CLIENTS 1000
#define MAX_ATTRACTIONS 10
#define DUREE_JOURNEE 60

//gestion du temps
time_t fin; // en seconde
time_t debut;


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
    //TODO Pour chaque attraction & l'accueil, recuperer le nombre de personne en file d'attente + le nombre de personne dedans
    //num=attractions[idat].capacite-file; ==> Pour affichage
    //Pour la file, faire un compteur de personne en file

    system("clear");
    printf("Client(s) dans le parc : %d\nFile d'attente d'entrée : %d\nNombre de clients dans les alléees : %d\n", nbClientsIn, nbClients-nbClientsIn-nbClientsOut, nbClientsInAllee);
    for(int i=0; i<MAX_ATTRACTIONS; i++){
        if (!attractions[i].libre){
            printf("Attraction n°%d : %d clients / %d clients \nFile d'attente de l'attraction : %d clients\n",attractions[i].numero, attractions[i].clientIn,attractions[i].capacite, attractions[i].file);
        }
        else{
            printf("Attraction n°%d : %d clients \n",attractions[i].numero, attractions[i].clientIn);
        }
    }
    printf("\n");


}

void *process_client(void *arg) {
    pthread_mutex_lock(&mutexEntree);
    //printf("Je suis le client n° %d et je rentre dans le parc\n", ((client*)arg)->numero);
    bool start = true;
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

                process_attraction(attractNum);

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


// possiblement faire un seul sleep pour touts les clients
void process_attraction(int idat) {
    //int num;

    //printf("Je rentre\n");
    if (attractions[idat].libre){
        attractions[idat].clientIn++;
        sleep(rand()%7);
        attractions[idat].clientIn--;
    }
    else{
        attractions[idat].file++;
        sem_wait(&attractions[idat].semaphore);
        attractions[idat].file--;
        attractions[idat].clientIn++;
        //printf("Attraction n° %d en cours\n", idat);

        sleep(attractions[idat].duree);


        //printf("Attraction n° %d finie\n", idat);
        sem_post(&attractions[idat].semaphore);

        attractions[idat].clientIn--;
    }
}






int main(int argc, char const *argv[]) {
    char prochainJour = 'n';

    srand(time(NULL));
    nbClients = rand()%MAX_CLIENTS+10;

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
        attractions[i].libre = rand()%2;
        attractions[i].capacite = rand()%10+1;
        attractions[i].duree = rand()%10+1;
        attractions[i].clientIn = 0;
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

        // affichage();

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



        printf("Nombre de clients aujourd'hui: %d\n", nbClients);


        caisse= caisse+caisseJour;
        nbClientsIn = 0;
        nbClientsOut = 0;
        nbClientsInAllee = 0;
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

