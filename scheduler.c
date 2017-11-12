#include <stdlib.h>
#include <stdio.h>
//#define _POSIX_C_SOURCE >= 199309L
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdint.h>
#include "queue.h"

// Global Queue pointers
queue *run_ptr;
queue *io_ptr;
queue *done_ptr;

// Run Queue locks
sem_t ready_lock;

// IO Queue locks
sem_t io_lock;

// Finished Queue locks
sem_t finished_lock;

sem_t counter_lock;
unsigned int counter = 1;
int stop = 0;

int wait(int time){
	struct timespec begin, end;
	clock_gettime(CLOCK_MONOTONIC,&begin);
	clock_gettime(CLOCK_MONOTONIC,&end);
	int elapsed=0;
	while (elapsed<=time) {
		clock_gettime(CLOCK_MONOTONIC, &end);
		elapsed = end.tv_sec-begin.tv_sec;
	}
	return 1;
}

void* job_submission_thread(void* arg){
	int thread = *((int*)arg);
    	free(arg);
    	int t;
	while(stop!=1){
		job* j =(job*) malloc(sizeof(job));
        	sem_wait(&counter_lock);
		init_job(j,counter);
        	counter++;
		sem_post(&counter_lock);
		sem_wait(&ready_lock);
		printf("Queueing new job, Submission Thread #: %d\n", thread);
		enqueue(run_ptr,j);
		sem_post(&ready_lock);
        	t =0;
		while (t!=1){
		    if(!isEmpty(done_ptr)){
               		sem_wait(&finished_lock);
                	if(isEmpty(done_ptr)){
                    		continue;
                	}
			printf("Removing finished job, Submit: %d\n", thread);
			job *ja= dequeue(done_ptr);
			delete_job(ja);
                	free(ja);
			sem_post(&finished_lock);
		    }
		    t = wait(2);
		}
	}
	printf("Submission Thread #: is exiting %d\n", thread);
	pthread_exit(0);
}

void * cpu_thread(void * arg) { 
	int thread = *((int*)arg);
   	free(arg);
	while (stop!=1) {
		if (!isEmpty(run_ptr)) {
			//sem_wait(&sub_run);
			sem_wait(&ready_lock);
       			if (isEmpty(run_ptr)) {
               			sem_post(&ready_lock);
                		continue;
          		}
			job* cpu = dequeue(run_ptr);
            		sem_post(&ready_lock);
			printf("Grabing a job for CPU #: %d\n",thread);
			//sem_post(&sub_run);
			printf("Job: %d, phase: %d complete, CPU #: %d\n",cpu->job_id, cpu->current_phase,thread);
			if (cpu->current_phase == cpu->tasks) {
				cpu->is_completed = 1;
				//sem_wait(&add_finished);
				sem_wait(&finished_lock);
				printf("Adding job: %d to Done Queue, CPU #: %d\n",cpu->job_id,thread);
				enqueue(done_ptr,cpu);
				//sem_post(&add_finished);
				sem_post(&finished_lock);
			}
			else if (cpu->phases[1][cpu->current_phase] == 0) {
				//sem_wait(&add_run);
				sleep(cpu->phases[0][cpu->current_phase]);
				cpu->current_phase++;
				sem_wait(&ready_lock);
				printf("Adding job to Run Queue, CPU #: %d\n", thread);
				enqueue(run_ptr,cpu);
				sem_post(&ready_lock);
				//sem_post(&add_run);
			}
			else {
				//sem_wait(&add_io);
				sem_wait(&io_lock);
				printf("Adding job to IO Queue, CPU #: %d\n",thread);
				enqueue(io_ptr,cpu);
				sem_post(&io_lock);
				//sem_post(&add_io);
			}
			
		}
	}
	
	printf("CPU #: is exiting %d\n",(int)thread);
	pthread_exit(0);
}

void * io_thread(void * arg) { 
	int thread = *((int*)arg);
    	free(arg);
	while (stop!=1) {
		if (!isEmpty(io_ptr)) {
			//sem_wait(&sub_run);
			sem_wait(&io_lock);
       			if (!isEmpty(io_ptr)) {
                		sem_post(&io_lock);
                		continue;
           	 	}
			job *io = dequeue(io_ptr);
			printf("Grabing a job for IO: %d\n", thread);
			sem_post(&io_lock);
			//sem_post(&sub_run);
			printf("Job phase: %d complete, IO #: %d\n", io->current_phase, thread);
			if (io->current_phase == io->tasks) {
				io->is_completed = 1;
				//sem_wait(&add_finished);
				sem_wait(&finished_lock);
				printf("Adding job to Done Queue, IO: %d\n", thread);
				enqueue(done_ptr,io);
				//sem_post(&add_finished);
				sem_post(&finished_lock);
			}else if (io->phases[1][io->current_phase] == 0) {
				//sem_wait(&add_run);
				sem_wait(&ready_lock);
				printf("Adding job to Run Queue, IO #: %d\n", thread);
				enqueue(run_ptr,io);
				sem_post(&ready_lock);
				//sem_post(&add_run);
			}
			else {
				//sem_wait(&add_io);
				sleep(io->phases[0][io->current_phase]);
				io->current_phase++;
				sem_wait(&io_lock);
				printf("Adding job to IO Queue, IO #: %d\n",thread);
				enqueue(io_ptr,io);
				sem_post(&io_lock);
				//sem_post(&add_io);
			}	
		}
	}
	printf("IO #: is exiting %d\n",(int)thread);
	pthread_exit(0);
}

int main() {
	// Initialization of Queue Locks
	sem_init(&ready_lock,0,1);
	sem_init(&io_lock,0,1);
	sem_init(&finished_lock,0,1);

	// Memory Allocation and Initialization for Queues
	run_ptr =(queue*) malloc(sizeof(queue));
	io_ptr =(queue*) malloc(sizeof(queue));
	done_ptr = (queue*)malloc(sizeof(queue));
	queue_init(run_ptr);
	queue_init(io_ptr);
	queue_init(done_ptr);

	srand(time(NULL));
	stop = 0;
	// global counter lock init
	sem_init(&counter_lock,0,1);
	
	// Job submission thread initialization
	pthread_t* job_submission = malloc(4*sizeof(pthread_t));
	for(int i = 0 ; i < 4; i++){
        	int *arg = malloc(sizeof(*arg));
        	*arg = i;
		int rc = pthread_create(&job_submission[i], NULL,job_submission_thread,arg);
		assert(rc == 0);
	}
	// CPU thread initialization
	pthread_t* cpu = malloc(8*sizeof(pthread_t));
	for (int i=0; i<8; i++) {
        	int *arg = malloc(sizeof(*arg));
        	*arg = i;
		int rc = pthread_create(&cpu[i],NULL,cpu_thread,arg);
		assert(rc == 0);
	}
	// IO thread initialization
	pthread_t* io = malloc(4*sizeof(pthread_t));
	for (int i=0; i<4; i++) {
        	int *arg = malloc(sizeof(*arg));
        	*arg = i;
		int rc = pthread_create(&io[i],NULL,io_thread,arg);
		assert(rc == 0);
	}
	wait(30);
	stop = 1;
	printf("Times UP!!!\n");
	for (int i=0; i<4; i++) {
		int rc = pthread_join(job_submission[i],NULL);
		assert(rc==0);
	}
	for (int i=0; i<8; i++) {
		int rc = pthread_join(cpu[i],NULL);
		assert(rc==0);
	}
	for (int i=0; i<4; i++) {
		int rc = pthread_join(io[i],NULL);
		assert(rc==0);
	}
	printf("# of Jobs: %d\n",counter-1);
	printf("DONE!!!!!!!\n");
	
	while(!isEmpty(run_ptr)){
		job *j = dequeue(run_ptr);
		delete_job(j);
       		free(j);
	}
	while(!isEmpty(io_ptr)){
		job *j = dequeue(io_ptr);
		delete_job(j);
        	free(j);
	}
	while(!isEmpty(done_ptr)){
		job *j = dequeue(done_ptr);
		delete_job(j);
        	free(j);
	}
	free(run_ptr);
	free(io_ptr);
	free(done_ptr);
	sem_destroy(&ready_lock);
	sem_destroy(&io_lock);
	sem_destroy(&finished_lock);
    	sem_destroy(&counter_lock);
    	free(job_submission);
    	free(cpu);
    	free(io);
	
	return 0;
}
