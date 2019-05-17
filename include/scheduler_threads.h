#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdint.h>
#include "queue.h"

#ifndef __SCHEDULER_THREADS_H__
#define __SCHEDULER_THREADS_H__


// Global Queue pointers
QUEUE *run_ptr;
QUEUE *io_ptr;
QUEUE *done_ptr;

// Run Queue locks
sem_t ready_full;
sem_t ready_empty;
sem_t ready_lock;

// IO Queue locks
sem_t io_full;
sem_t io_empty;
sem_t io_lock;

// Finished Queue locks
sem_t finished_full;
sem_t finished_empty;
sem_t finished_lock;

sem_t counter_lock;
sem_t job_counter_lock;

// Job Book keeping
unsigned int counter;
unsigned int job_counter;

// Stops scheduler
int stop;

// Threads
pthread_t* job_submission;
pthread_t* cpu;
pthread_t* io;


int wait(int time);

void schedule_init(void);

void schedule_thread_start(void);

void schedule_thread_join(void);

void schedule_destroy(void);

void* job_submission_thread(void* arg);

void* cpu_thread(void * arg);

void* io_thread(void* arg);


#endif