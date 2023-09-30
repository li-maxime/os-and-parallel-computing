#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "stream_common.h"
#include "oggstream.h"



int main(int argc, char *argv[]) {
    pthread_t thread_video, thread_audio;
    int res;

    /*if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);*/
    //char *filename = argv[1];
    char *filename = "/home/maxime/Desktop/SEPC/ensimag-video-master/build/Big_Buck_Bunny_medium.ogv";


    printf("> Initialisation SDL ... ");
    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);
    printf("DONE\n");
    fflush(stdout);

    // start the two stream readers
    printf("> Initialisation stream readers ... ");
    if (pthread_create(&thread_video, NULL, theoraStreamReader, filename)){
        printf("erreur thread video\n");
        exit(-1);
    }
    if (pthread_create(&thread_audio, NULL, vorbisStreamReader, filename)){
        printf("erreur thread audio\n");
        exit(-1);
    }


    printf("DONE\n");
    
    // wait audio thread
    printf("Attendre thread audio");
    pthread_join(thread_audio, NULL);
    printf("Réussi");
    fflush(stdout);


    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    printf("Kill video thread");
    pthread_cancel(thread_video);
    pthread_cancel(theora2sdlthread);

    // attendre les 2 threads videos
    pthread_join(thread_video, NULL);
    pthread_join(theora2sdlthread, NULL);

    
    exit(EXIT_SUCCESS);    
}
