#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>


extern bool fini;

pthread_mutex_t hashmap_mutex = PTHREAD_MUTEX_INITIALIZER;

/* les variables pour la synchro, ici */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_taille_fenetre = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_pret = PTHREAD_COND_INITIALIZER;


int compteur = 0;
bool pret = false;
bool use = false;


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mutex);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    pthread_cond_signal(&cond_taille_fenetre);
    pthread_mutex_unlock(&mutex);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&mutex);
    while(windowsx == 0){
        pthread_cond_wait(&cond_taille_fenetre, &mutex);
    }
    pthread_mutex_unlock(&mutex);

}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mutex);
    pret = true;
    pthread_cond_signal(&cond_pret);
    pthread_mutex_unlock(&mutex);

}

void attendreFenetreTexture() {
    pthread_mutex_lock(&mutex);
    while(!pret){
        pthread_cond_wait(&cond_pret, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&mutex);
    while ((compteur == 0) && !use){
        pthread_cond_wait(&cond_cons, &mutex);
    }
    compteur -= 1;
    use = true;
    pthread_mutex_unlock(&mutex);
}

void finConsommerTexture() {
    pthread_mutex_lock(&m);
    use = false;
    pthread_cond_signal(&cond_prod);
    pthread_mutex_unlock(&m);


}


void debutDeposerTexture() {
    pthread_mutex_lock(&mutex);
    while ((compteur == 1) && !use){
        pthread_cond_wait(&cond_prod, &mutex);
    }
    compteur += 1;
    use = true;
    pthread_mutex_unlock(&mutex);
}

void finDeposerTexture() {
    pthread_mutex_lock(&m);
    use = false;
    pthread_cond_signal(&cond_cons);
    pthread_mutex_unlock(&m);
}
