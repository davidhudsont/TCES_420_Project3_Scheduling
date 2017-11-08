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

void * cpu_thread(void * arg) { 
	int*  thread = (int*)arg;
	while (stop!=1) {
		if (!isEmpty(run_ptr)) {
			//sem_wait(&sub_run);
			sem_wait(&sub_run_lock);
			job* cpu = dequeue(run_ptr);
			if (cpu== NULL) {
				sem_post(&sub_run_lock);
				printf("Stuff Stuff\n");
				continue;
			}
			printf("Grabing a job, CPU: %d\n",(int)thread);
			sem_post(&sub_run_lock);
			//sem_post(&sub_run);
			sleep(cpu->phases[0][cpu->current_phase]);
			printf("Job phase: %d complete, CPU: %d\n", cpu->current_phase,(int)thread);
			cpu->current_phase++;
			if (cpu->current_phase == cpu->tasks) {
				cpu->is_completed = 1;
				//sem_wait(&add_finished);
				sem_wait(&add_finished_lock);
				printf("Adding job to finished Queue, CPU: %d\n",(int)thread);
				enqueue(done_ptr,cpu);
				//sem_post(&add_finished);
				sem_post(&add_finished_lock);
			}
			if (cpu->phases[1][cpu->current_phase] == 0) {
				//sem_wait(&add_run);
				sem_wait(&add_run_lock);
				printf("Adding job to Run Queue, CPU: %d\n", (int)thread);
				enqueue(run_ptr,cpu);
				sem_post(&add_run_lock);
				//sem_post(&add_run);
			}
			if (cpu->phases[1][cpu->current_phase] == 1) {
				//sem_wait(&add_io);
				sem_wait(&add_io_lock);
				printf("Adding job to IO Queue, CPU: %d\n",(int)thread);
				enqueue(io_ptr,cpu);
				sem_post(&add_io_lock);
				//sem_post(&add_io);
			}
			
		}
	}
	
	printf("Times UP!!!, CPU: %d\n",(int)thread);
	pthread_exit(0);
}
void* job_submission_thread(void* arg){
	int*  thread = (int*)arg;
	while(stop!=1){
		job* j =(job*) malloc(sizeof(job));
		sem_wait(&counter_lock);
		init_job(j,counter);
		sem_wait(&add_run_lock);
		printf("Queueing new job, Job submit thread: %d\n", (int)thread);
		enqueue(run_ptr,j);
		sem_post(&add_run_lock);
		counter++;
		sem_post(&counter_lock);
		int t =0;
		while (t!=1){
		    if(!isEmpty(done_ptr)){
			sem_wait(&sub_finished_lock);
			printf("Removing finished job, Submit: %d\n", (int)thread);
			j = dequeue(done_ptr);
            		delete_job(j);
            		free(j);
			sem_post(&sub_finished_lock);
		    }
		    t = wait(1);
		}
	}
	printf("Times UP!!!, Submit: %d\n",(int)thread);
	pthread_exit(0);
}

void * io_thread(void * arg) { 
	int*  thread = (int*)arg;
	while (stop!=1) {
		if (!isEmpty(io_ptr)) {
			//sem_wait(&sub_run);
			sem_wait(&sub_io_lock);
			job *io = dequeue(io_ptr);
			if (io==NULL ) {
				sem_post(&sub_io_lock);
				continue;
			}
			printf("Grabing a job,  IO: %d\n",(int)thread);
			sem_post(&sub_io_lock);
			//sem_post(&sub_run);
			sleep(io->phases[0][io->current_phase]);
			printf("Job phase: %d complete, IO: %d\n", io->current_phase,(int)thread);
			io->current_phase++;
			if (io->current_phase == io->tasks) {
				io->is_completed = 1;
				//sem_wait(&add_finished);
				sem_wait(&add_finished_lock);
				printf("Adding job to finished Queue, IO: %d\n",(int)thread);
				enqueue(done_ptr,io);
				//sem_post(&add_finished);
				sem_post(&add_finished_lock);
			}
			if (io->phases[1][io->current_phase] == 0) {
				//sem_wait(&add_run);
				sem_wait(&add_run_lock);
				printf("Adding job to Run Queue, IO: %d\n", (int)thread);
				enqueue(run_ptr,io);
				sem_post(&add_run_lock);
				//sem_post(&add_run);
			}
			if (io->phases[1][io->current_phase] == 1) {
				//sem_wait(&add_io);
				sem_wait(&add_io_lock);
				printf("Adding job to CPU Queue, IO: %d\n",(int)thread);
				enqueue(io_ptr,io);
				sem_post(&add_io_lock);
				//sem_post(&add_io);
			}	
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
	run_ptr =(queue*) malloc(sizeof(queue));
	io_ptr =(queue*) malloc(sizeof(queue));
	done_ptr = (queue*)malloc(sizeof(queue));
	queue_init(run_ptr,qSIZE);
	queue_init(io_ptr,qSIZE);
	queue_init(done_ptr,qSIZE);

	srand(time(NULL));
	printf("Before stupid loop\n");
	stop = 0;
	//***********************thien Nguyen ******************************///
	// global counter lock init
	sem_init(&counter_lock,0,1);
	// Job submission thread initialization
	pthread_t job_submission[4];
	for(int i = 0 ; i < 4; i++){
		int rc = pthread_create(&job_submission[i], NULL,job_submission_thread,(void *)i );
		assert(rc == 0);
	}
	// CPU thread initialization
	pthread_t cpu[8];
	for (int i=0; i<8; i++) {
		int rc = pthread_create(&cpu[i],NULL,cpu_thread,(void *)i);
		assert(rc == 0);
	}
	// IO thread initialization
	pthread_t io[4];
	for (int i=0; i<4; i++) {
		int rc = pthread_create(&io[i],NULL,io_thread,(void *)i);
		assert(rc == 0);
	}
	wait(60);
	stop = 1;
	printf("Times UP!!!\n");
	for (int i=0; i<4; i++) {
		int rc = pthread_join(job_submission[i],NULL);
		assert(rc==0);
	}
	printf("What happened?");
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
	/*
	printf("After join\n");
	for (int i=0; i<4; i++) {
		int rc = pthread_create(&cpu,NULL,io_thread,arg);
		asssert(rc == 0);
	}
	*/
	/*
	for (int i=0; i<run_ptr->qcapacity; i++) {
		free(run_ptr->jobqueue[i].phases[0]);
		free(run_ptr->jobqueue[i].phases[1]);
		free(run_ptr->jobqueue[i].phases);
		
	}	
	for (int i=0; i<io_ptr->qcapacity; i++) {
		free(io_ptr->jobqueue[i].phases[0]);
		free(io_ptr->jobqueue[i].phases[1]);
		free(io_ptr->jobqueue[i].phases);
	}
	for (int i=0; i<done_ptr->qcapacity; i++) {
		free(done_ptr->jobqueue[i].phases[0]);
		free(done_ptr->jobqueue[i].phases[1]);
		free(done_ptr->jobqueue[i].phases);
	}
	*/
	/*
	for (int i=0; i<run_ptr->qsize; i++) {
		free(run_ptr->jobqueue[i].phases[0]);
		free(run_ptr->jobqueue[i].phases[1]);
		free(run_ptr->jobqueue[i].phases);
		
	}	
	for (int i=0; i<io_ptr->qsize; i++) {
		free(io_ptr->jobqueue[i].phases[0]);
		free(io_ptr->jobqueue[i].phases[1]);
		free(io_ptr->jobqueue[i].phases);
	}
	for (int i=0; i<done_ptr->qsize; i++) {
		free(done_ptr->jobqueue[i].phases[0]);
		free(done_ptr->jobqueue[i].phases[1]);
		free(done_ptr->jobqueue[i].phases);
	}
	free(run_ptr->jobqueue);
	free(io_ptr->jobqueue);
	free(done_ptr->jobqueue);
	free(run_ptr);
	free(io_ptr);
	free(done_ptr);
	*/
	return 0;
}
