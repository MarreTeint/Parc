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

void *process_client(void *arg) {
    pthread_mutex_lock(&mutexEntree);
    // printf("Je suis le client n° %d et je rentre dans le parc\n", ((client*)arg)->numero);
    /*printf("J'ai payé les 30 euros !\n");*/
    pthread_mutex_unlock(&mutexEntree);
    while (true){
        int choix = rand()%3;
        switch (choix)
        {
        case 0:
            // printf("Je suis le client n° %d et je sors du parc\n", ((client*)arg)->numero);
            pthread_exit(NULL);
            break;
        
        case 1: ;
            int attractNum = rand()%MAX_ATTRACTIONS;
            //process attraction
            // printf("Je suis le client n° %d et je vais à l'attraction n° %d\n", ((client*)arg)->numero, attractNum);
            process_attraction(attractNum);
            //Passe en suite dans l'allée à la fin de son attraction
        
        case 2: ;
            int howLong = rand()%5;
            // printf("Je suis le client n° %d et je reste dans l'allée %d secondes\n", ((client*)arg)->numero, howLong);
            sleep(howLong);
            break;
        }
            
    }
}

void process_attraction(int idat) {
    // sem_wait(&attractions[idat].semaphore); //faire passer chez client les attractions
    // //While(sem != 0 AND file d'attente != 0){wait()}
    // if (attractions[idat].libre){
    //     sleep(rand()%7);
    // }
    // else{
    //     sleep(attractions[idat].duree);
    // }
    // sem_post(&attractions[idat].semaphore);
}

/*void *process_attraction(void *arg) {
    // sem_wait(& attraction.semaphore ici)
    //While(sem != 0 AND file d'attente != 0){wait()}
    sleep(1);//Mettre attraction.duree
    pthread_exit(NULL);
}*/

void affichage(){
    //TODO Pour chaque attraction & l'accueil, recuperer le nombre de personne en file d'attente + le nombre de personne dedans
}

int main(int argc, char const *argv[]) {
    char prochainJour = 'n';
    int caisse = 0;
    srand(time(NULL));
    int nbClients = rand()%MAX_CLIENTS+10;
    int caisseJour;
    int nbJour = 0;
    client clients[nbClients];
    attraction attractions[MAX_ATTRACTIONS];

    //initialisation des attractions et des clients 
    for (int i = 0; i < MAX_ATTRACTIONS; i++) {
        attractions[i].numero = i;
        if (i%2 == 0) {
            attractions[i].libre = 1;
        }
        attractions[i].libre = 0;

        sem_init(&attractions[i].semaphore, 0, 1);

    }//pas oublier de vider les actractions à la fin du jour

    for (int i = 0; i < nbClients; i++) {
        // sleep(0.01);
        clients[i].numero = i;
        clients[i].satisfaction = 100;
        clients[i].argent = 100;
        pthread_create(&clients[i].thread, NULL, &process_client, &clients[i]);
    }

    printf("Nombre de clients aujourd'hui: %d\n", nbClients);


    do {
        nbJour++;

        time_t fin;//en seconde
        time_t debut;
        fin = time(NULL) + 5;
        debut = time(NULL);
        caisseJour = 0;
        printf("Nombre de clients aujourd'hui: %d\n", nbClients);

        while ((fin - debut) > 0) {
            debut = time(NULL);
        
        }

        printf("Fin journée n° %d, Chiffre d'affaires du jour : %d €\n", nbJour, caisseJour);
        
        printf("Jour suivant ? (o/n) \n");
        scanf(" %c", &prochainJour);
    } while  (prochainJour == 'o');  
    // en fonction du taux de satisfaction => si inf à 50 : 1/2 de revenir etc
    //TODO déterminer les horraires du parc (pas de 24h/24)


    //Fin de la journée
    for (int i = 0; i < nbClients; i++) {
        pthread_join(clients[i].thread, NULL);
    }
    caisse+=caisseJour;

    printf("Caisse du parc au bout de %d jour(s): %d € \n",nbJour, caisse);
    return 0;
}


