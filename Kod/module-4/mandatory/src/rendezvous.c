/**
 * Rendezvous
 *
 * Two threads executing chunks of work in lock step.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* abort(), [s]rand() */
#include <unistd.h>    /* sleep() */
#include <semaphore.h> /* sem_...() */
#include <pthread.h>   /* pthread_...() */

#define LOOPS 5
#define NTHREADS 3
#define MAX_SLEEP_TIME 3

sem_t a_semaphore;
sem_t b_semaphore;


/* TODO: Make the two threads perform their iterations in lockstep. */

void *
threadA(void *param __attribute__((unused)))
{
    int i;

    for (i = 0; i < LOOPS; i++) {

        // wait
        sem_wait(&b_semaphore);
        printf("A%d\n", i);
        sleep(rand() % MAX_SLEEP_TIME);
        sem_post(&a_semaphore);

    }

    pthread_exit(0);
}


/* TODO: Make the two threads perform their iterations in lockstep. */

void *
threadB(void *param  __attribute__((unused)))
{
    int i;

    for (i = 0; i < LOOPS; i++) {

        sem_wait(&a_semaphore);

        printf("B%d\n", i);
        sleep(rand() % MAX_SLEEP_TIME);
        sem_post(&b_semaphore);

    }

pthread_exit(0);
}

int
main()
{
    pthread_t tidA, tidB;
    sem_init(&a_semaphore, 0,1 );
    sem_init(&b_semaphore, 0,1 );
    srand(time(NULL));
    pthread_setconcurrency(3);

    if (pthread_create(&tidA, NULL, threadA, NULL) ||
	pthread_create(&tidB, NULL, threadB, NULL)) {
	perror("pthread_create");
	abort();
    }
    if (pthread_join(tidA, NULL) != 0 ||
        pthread_join(tidB, NULL) != 0) {
	perror("pthread_join");
	abort();
    }

    return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 */
