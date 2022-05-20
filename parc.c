#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
//constante pour le nombre de threads clients max, la journée dans le parc, nb attractions
#define MAX_CLIENTS 100
#define MAX_ATTRACTIONS 10

pthread_mutex_t mutexEntree = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int numero;
    int satisfaction;
    pthread_t thread;
    int argent;
} client;

typedef struct {
    int numero;
    int libre;
    sem_t semaphore;
} attraction;

void *process_client(void *arg) {
    pthread_mutex_lock(&mutexEntree);
    printf("Je suis le client n° %d\n", ((client*)arg)->numero);
    printf("J'ai payé les 30 euros !\n");
    pthread_mutex_unlock(&mutexEntree);    
    pthread_exit(NULL);
}

void *process_attraction(void *arg) {
    sleep(1);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    //faire un while true pour relancer des journées
    // en fonction du taux de satisfaction => si inf à 50 : 1/2 de revenir etc
	int nbClients = rand()%MAX_CLIENTS+10;

    client clients[nbClients];
    attraction attractions[MAX_ATTRACTIONS];

    for (int i = 0; i < MAX_ATTRACTIONS; i++) {
        attractions[i].numero = i;
        if (i%2 == 0) {
            attractions[i].libre = 1;
        }
        attractions[i].libre = 0;

        sem_init(&attractions[i].semaphore, 0, 1);

    }

    printf("Nombre de clients: %d\n", nbClients);
    for (int i = 0; i < nbClients; i++) {
        // sleep(0.01);
        clients[i].numero = i;
        clients[i].satisfaction = 100;
        clients[i].argent = 100;
        pthread_create(&clients[i].thread, NULL, &process_client, &clients[i]);
    }
    

    //Fin de la journée
    for (int i = 0; i < nbClients; i++) {
        pthread_join(clients[i].thread, NULL);
    }

    //TODO pas oublier le prix d'entrée

    return 0;
}



