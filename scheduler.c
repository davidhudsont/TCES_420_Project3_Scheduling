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
unsigned int counter = 1;
unsigned int job_counter = 0;
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
		// Producer
		
    	sem_wait(&counter_lock);
		job *j = init_job(counter);
    	counter++;
		sem_post(&counter_lock);

		// sem_wait(&ready_empty);
		sem_wait(&ready_lock);
		printf("Job: %d queued, Submission Thread #: %d\n",j->job_id, thread);
		run_ptr->enqueue(run_ptr,j);
		sem_post(&ready_lock);
		// sem_post(&ready_full);

        t =0;
        // Consumer
		while (t!=1){
		    if(!isEmpty(done_ptr)){
		    	// sem_wait(&finished_full);
           		sem_wait(&finished_lock);
            	if (isEmpty(done_ptr)){
					sem_post(&finished_lock);
					// sem_post(&finished_empty);
            		continue;
            	}
				job *ja= done_ptr->dequeue(done_ptr);
				printf("Job: %d Complete!, Submission Thread #: %d\n",ja->job_id, thread);
				ja->destroy(ja);
				job_counter++;
				sem_post(&finished_lock);
				// sem_post(&finished_empty);
		    }
		    t = wait(5);
		}
	}
	printf("Submission Thread #: %d is exiting\n", thread);
	pthread_exit(0);
}

void * cpu_thread(void * arg) { 
	int thread = *((int*)arg);
   	free(arg);
	while (stop!=1) {
		if (!isEmpty(run_ptr)) {
			// Consumer
			// sem_wait(&ready_full);
			sem_wait(&ready_lock);
   			if (isEmpty(run_ptr)) {
           			sem_post(&ready_lock);
           			// sem_post(&ready_empty);
            		continue;
      		}
			job *cpu = run_ptr->dequeue(run_ptr);
    		sem_post(&ready_lock);
    		// sem_post(&ready_empty);

    		if (cpu->task_type == 0) {
    			cpu->complete(cpu);
				cpu->next(cpu);
    		}
    		// Producer
			if (cpu->is_completed == 1) {
				// sem_wait(&finished_empty);
				sem_wait(&finished_lock);
				printf("Finished Job: %d pushed to Done Queue, CPU Thread #: %d\n",cpu->job_id,thread);
				done_ptr->enqueue(done_ptr,cpu);
				sem_post(&finished_lock);
				// sem_post(&finished_full);
			}
			else if (cpu->task_type == 0) {
				// sem_wait(&ready_empty);
				sem_wait(&ready_lock);
				printf("Job: %d pushed to Run Queue, CPU Thread #: %d\n",cpu->job_id, thread);
				run_ptr->enqueue(run_ptr,cpu);
				sem_post(&ready_lock);
				// sem_post(&ready_full);
			}
			else {
				// sem_wait(&io_empty);
				sem_wait(&io_lock);
				printf("Job: %d pushed to IO Queue, CPU Thread #: %d\n",cpu->job_id,thread);
				io_ptr->enqueue(io_ptr,cpu);
				sem_post(&io_lock);
				// sem_post(&io_full);
			}
			
		}
	}
	
	printf("CPU Thread #: is exiting %d\n",thread);
	pthread_exit(0);
}

void * io_thread(void * arg) { 
	int thread = *((int*)arg);
    	free(arg);
	while (stop!=1) {
		if (!isEmpty(io_ptr)) {
			// Consumer
			// sem_wait(&io_full);
			sem_wait(&io_lock);
   			if (!isEmpty(io_ptr)) {
            		sem_post(&io_lock);
            		// sem_post(&io_empty);
            		continue;
       	 	}
			job *io = io_ptr->dequeue(io_ptr);
			sem_post(&io_lock);
			// sem_post(&io_empty);
			// Producers
			if (io->task_type == 1){
				io->complete(io);
				io->next(io);
			}				
			if (io->is_completed == 1) {
				// sem_wait(&finished_empty);
				sem_wait(&finished_lock);
				printf("Finished Job: %d pushed to Done Queue, IO Thread #: %d\n", io->job_id, thread);
				done_ptr->enqueue(done_ptr,io);
				sem_post(&finished_lock);
				// sem_post(&finished_full);

			}else if (io->task_type == 0) {
				// sem_wait(&ready_empty);
				sem_wait(&ready_lock);
				printf("Job: %d pushed to Run Queue, IO Thread #: %d\n", io->job_id,thread);
				run_ptr->enqueue(run_ptr,io);
				sem_post(&ready_lock);
				// sem_post(&ready_full);
			}

			else {
				// sem_wait(&io_empty);
				sem_wait(&io_lock);
				printf("Job: %d pushed to IO Queue, IO Thread #: %d\n",io->job_id,thread);
				io_ptr->enqueue(io_ptr,io);
				sem_post(&io_lock);
				// sem_post(&io_full);
			}	
		}
	}
	printf("IO Thread #: is exiting %d\n",thread);
	pthread_exit(0);
}

int main(int argc, char *argv[]) {

	printf("argc: %d\n",argc);
	int runtime = 30;
	if (argc == 2) {
		runtime = atoi(argv[1]);
		printf("Run time %d\n", runtime);
	}
	

	// SEED
	srand(time(NULL));

	// Initialization of Queue Locks
	sem_init(&ready_empty,0,16);
	sem_init(&ready_full,0,0);
	sem_init(&ready_lock,0,1);


	sem_init(&io_empty,0,8);
	sem_init(&io_full,0,0);
	sem_init(&io_lock,0,1);

	sem_init(&finished_empty,0,8);
	sem_init(&finished_full,0,0);
	sem_init(&finished_lock,0,1);

	// Memory Allocation and Initialization for Queues
	run_ptr = queue_init(run_ptr);
	io_ptr = queue_init(io_ptr);
	done_ptr = queue_init(done_ptr);

	
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
	wait(60);
	stop = 1;
		
	//printf("Times UP!!!\n");
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
	printf("# of Jobs Created: %d\n",counter-1);
	printf("# of Jobs Finished %d\n",job_counter);
	double percent = (double) job_counter/(counter-1)*100.0;
	printf("Percentage Complted %f%%\n",percent );
	printf("DONE!!!!!!!\n");
	
	sem_destroy(&ready_lock);
	sem_destroy(&io_lock);
	sem_destroy(&finished_lock);
    sem_destroy(&counter_lock);
    sem_destroy(&ready_empty);
    sem_destroy(&ready_full);
    sem_destroy(&io_empty);
    sem_destroy(&io_full);
    sem_destroy(&finished_full);
    sem_destroy(&finished_empty);

    free(job_submission);
	free(cpu);
	free(io);

	run_ptr->destroy(run_ptr);
	io_ptr->destroy(io_ptr);
	done_ptr->destroy(done_ptr);
  
	return 0;
}
