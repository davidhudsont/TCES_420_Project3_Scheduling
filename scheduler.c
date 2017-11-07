#include <stdlib.h>
#include <stdio.h>
//#define _POSIX_C_SOURCE >= 199309L
#include <time.h>
#include <queue.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdint.h>

// Queue and Job size
#define qSIZE 100
#define jSIZE 16

// Global Queue pointers
queue *run_ptr;
queue *io_ptr;
queue *done_ptr;

// Run Queue locks
sem_t add_run;
sem_t add_run_lock;
sem_t sub_run;
sem_t sub_run_lock;

// IO Queue locks
sem_t add_io;
sem_t add_io_lock;
sem_t sub_io;
sem_t sub_io_lock;

// Finished Queue locks
sem_t add_finished;
sem_t add_finished_lock;
sem_t sub_finished;
sem_t sub_finished_lock;

sem_t counter_lock;
unsigned int counter = 0;

void * cpu_thread(void * arg) { 
	int*  thread = (int*)arg;
	while (1) {
		if (!isEmpty(run_ptr)) {
			//sem_wait(&sub_run);
			int value;
			sem_wait(&sub_run_lock);
			job cpu = dequeue(run_ptr);
			if (cpu.job_id < 0) {
				sem_post(&sub_run_lock);
				continue;
			}
			printf("Grabing a job, Thread number: %d\n",(int)thread);
			sem_post(&sub_run_lock);
			//sem_post(&sub_run);
			sleep(cpu.phases[0][cpu.current_phase]);
			printf("Job phase: %d complete, Thread number: %d\n", cpu.current_phase,(int)thread);
			cpu.current_phase++;
			if (cpu.current_phase == cpu.tasks) {
				cpu.is_completed = 1;
				//sem_wait(&add_finished);
				sem_wait(&add_finished_lock);
				printf("Adding job to finished Queue, Thread number: %d\n",(int)thread);
				enqueue(done_ptr,cpu);
				//sem_post(&add_finished);
				sem_post(&add_finished_lock);
			}
			if (cpu.phases[1][cpu.current_phase] == 0) {
				//sem_wait(&add_run);
				sem_wait(&add_run_lock);
				printf("Adding job to Run Queue, Thread number: %d\n", (int)thread);
				enqueue(run_ptr,cpu);
				sem_post(&add_run_lock);
				//sem_post(&add_run);
			}
			if (cpu.phases[1][cpu.current_phase] == 1) {
				//sem_wait(&add_io);
				sem_wait(&add_io_lock);
				printf("Adding job to IO Queue, Thread number: %d\n",(int)thread);
				enqueue(io_ptr,cpu);
				sem_post(&add_io_lock);
				//sem_post(&add_io);
			}
			
		}
	}
	pthread_exit(0);
}
void* job_submission_thread(void* arg){
	int*  thread = (int*)arg;
	struct timespec begin, end;
	while(1){
		job* j = malloc(sizeof(job));
		init_job(j,counter);
		sem_wait(&add_run_lock);
		enqueue(run_ptr,*j);
		free(j);
		sem_post(&add_run_lock);
		sem_wait(&counter_lock);
		counter++;
		sem_post(&counter_lock);
		clock_gettime(CLOCK_MONOTONIC, &begin);
		clock_gettime(CLOCK_MONOTONIC, &end);
		while ((end.tv_sec - begin.tv_sec) >= 2){
		    sem_wait(&add_finished_lock);
		    if(!isEmpty(done_ptr)){
			dequeue(done_ptr);
		    }
		    sem_post(&add_finished_lock);
		    clock_gettime(CLOCK_MONOTONIC, &end);
		}
	}
	pthread_exit(0);
}

int main() {
	// Initialization of Queue Locks
	sem_init(&add_run,0,qSIZE);
	sem_init(&add_run_lock,0,1);
	sem_init(&sub_run,0,qSIZE);
	sem_init(&sub_run_lock,0,1);
	sem_init(&add_io,0,qSIZE);
	sem_init(&add_io_lock,0,1);
	sem_init(&sub_io,0,qSIZE);
	sem_init(&sub_io_lock,0,1);
	sem_init(&add_finished,0,qSIZE);
	sem_init(&add_finished_lock,0,1);
	sem_init(&sub_finished,0,qSIZE);
	sem_init(&sub_finished_lock,0,1);

	// Memory Allocation and Initialization for Queues
	run_ptr = malloc(sizeof(queue));
	io_ptr = malloc(sizeof(queue));
	done_ptr = malloc(sizeof(queue));
	queue_init(run_ptr,qSIZE);
	queue_init(io_ptr,qSIZE);
	queue_init(done_ptr,qSIZE);

	srand(time(NULL));
	printf("Before stupid loop\n");

	//***********************thien Nguyen ******************************///
	// global counter lock init
	sem_init(&counter_lock,0,1);
	// Job submission thread initialization
	pthread_t job_submission[4];
	for(int i = 0 ; i <= 4; i++){
		int rc = pthread_create(&job_submission[i], NULL,job_submission_thread,(void *)i );
		assert(rc == 0);
	}

	// pthread_t io[4];
	// CPU thread initialization
	pthread_t cpu[8];
	for (int i=0; i<8; i++) {
		int rc = pthread_create(&cpu[i],NULL,cpu_thread,(void *)i);
		assert(rc == 0);
	}	
	//sleep(5);
	/*
	for (int i=0; i<8; i++) {
		int rc = pthread_join(cpu[i],NULL);
		assert(rc==0);
	}

	printf("After join\n");
	for (int i=0; i<4; i++) {
		int rc = pthread_create(&cpu,NULL,io_thread,arg);
		asssert(rc == 0);
	}
	*/
	
	queue_delete(run_ptr);
	queue_delete(io_ptr);
	queue_delete(done_ptr);
	
	return 0;
}
