#include "appointment.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

static sem_t connected_lock;
static sem_t operators;
static int NUM_OPERATORS = 4;
static int NUM_LINES = 7;
static int connected = 0;
static int next_id = 1;

/**
 * @brief main function that takes in command line arguments to represent the 
 * function of threads in a CPU. This function initializes the pthread_t array and 
 * the binary and nary semaphores. 
 * @return int - 0
 */
int main(int argc, char **argv) {

    //Error checking for inputs that are not just 1 number
    if (argc != 2) {
        printf("Usage: %s <Number of phone calls>\n", argv[0]);
        return 0;
    }

    //string to int
    int call = atoi(argv[1]);

    //initializing pthread_t array and semaphores
    pthread_t thread[call];
    sem_init(&connected_lock, 0, 1);
    sem_init(&operators, 0, 4);

    //creates pthreads
    for (int i = 0; i < call; i++) {
        pthread_create(&thread[i], NULL, phonecall, NULL);
    }

    //joining the threads that we have created
    for (int i = 0; i < call; i++) {
        pthread_join(thread[i], NULL);
    }

    //destroying the semaphores we have created at the end of the program
    sem_destroy(&connected_lock);
    sem_destroy(&operators);

    return 0;
}


/**
 * @brief void function of phonecall that handles the threads and uses semaphores to
 * create critical sections of code. Ensures that only 4 operators and 7 lines are 
 * used at all times. 
 * No return as it is a void function.
 * 
 */
void *phonecall(void *vargp) {

    //critical section for updating the id of each caller
    sem_wait(&connected_lock);
    int id = next_id;
    next_id++;
    sem_post(&connected_lock);

    printf("Thread %d is attempting to connect.\n", id);

    //if/else statement deciding if there is an open line for a connection
    if (connected != NUM_LINES) { //if there is an available line to connect to

        //critical section for incrementing connected
        sem_wait(&connected_lock);
        connected++;
        sem_post(&connected_lock);
        printf("Thread %d connects to an available line, call ringing.\n", id);
    } else { // else wait for an open line to connect to
        printf("Line is busy.\n");

        //while loop waiting for open line
        while (true) { 

            //critical section for incrementing connected only when there is an open line
            sem_wait(&connected_lock);
            if (connected != NUM_LINES) {
                connected++;
                sem_post(&connected_lock);
                break;
            } 
            sem_post(&connected_lock);    
        }
        
        printf("Thread %d connects to an available line, call ringing.\n", id);
    }

    //critical section using nary semaphore of operators (counting semaphore)
    sem_wait(&operators);
    printf("Thread %d is speaking to an operator in the local health center.\n", id);
    sleep(3);
    printf("Thread %d has made an appointment for the test! The operator has left.\n", id);
    sem_post(&operators);

    //decrements connected after a thread has hung up (uses critical section)
    printf("Thread %d has hung up.\n", id);
    sem_wait(&connected_lock);
    connected--;
    sem_post(&connected_lock);

    pthread_exit(NULL);

}