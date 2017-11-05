#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define qSIZE 100
#define jSIZE 40

// unsigned int sleep(unsigned int seconds);
queue run;
queue io;
queue done;
queue *run_ptr = &run;
queue *io_ptr = &io;
queue *done_ptr = &done;

sem_t add_run;
sem_t add_run_lock;
sem_t sub_run;
sem_t sub_run_lock;
sem_init(&add_run,0,qSIZE);
sem_init(&add_run_lock,0,1);
sem_init(&sub_run,0,qSIZE);
sem_init(&add_run_lock,0,1);

sem_t add_io;
sem_t add_io_lock;
sem_t sub_io;
sem_t sub_io_lock;
sem_init(&add_io,0,qSIZE);
sem_init(&add_io_lock,0,1);
sem_init(&sub_io,0,qSIZE);
sem_init(&sub_io_lock,0,1);

sem_t add_finished;
sem_t add_finished_lock;
sem_t sub_finished;
sem_t sub_finished_lock;
sem_init(&add_finished,0,qSIZE);
sem_init(&add_finished_lock,0,1);
sem_init(&sub_finished,0,qSIZE);
sem_init(&sub_finished_lock,0,1);

/*
		sem_wait(&sub_run);
		sem_wait(&sub_lock);
		job cpu = dequeue(run_ptr);
		sem_post(&sub_lock);
		sem_post(&sub_run);
*/
void * cpu_thread(void * arg) { 
	int*  thread = (int*)arg;
	while (1==1) {
		if (!isEmpty(run_ptr)) {
			sem_wait(&sub_run);
			sem_wait(&sub_run_lock);
			job cpu = dequeue(run_ptr);
			printf("Grabing a job, Thread number: %d\n",(int)thread);
			sem_post(&sub_run_lock);
			sem_post(&sub_run);
			sleep(cpu.phases[0][cpu.current_phase]);
			printf("Job phase: %d complete, Thread number: %d\n", cpu.current_phase,(int)thread);
			cpu.current_phase++;
			if (cpu.current_phase == cpu.tasks) {
				cpu.is_completed = 1;
				sem_wait(&add_finished);
				sem_wait(&add_finished_lock);
				printf("Adding job to finished Queue, Thread number: %d\n",(int)thread);
				enqueue(done_ptr,cpu);
				sem_post(&add_finished);
				sem_post(&add_finished_lock);
			}
			if (cpu.phases[1][cpu.current_phase] == 0) {
				sem_wait(&add_run);
				sem_wait(&add_run_lock);
				printf("Adding job to Run Queue, Thread number: %d\n", (int)thread);
				enqueue(run_ptr,cpu);
				sem_post(&add_run_lock);
				sem_post(&add_run);
			}
			if (cpu.phases[1][cpu.current_phase] == 1) {
				sem_wait(&add_io);
				sem_wait(&add_io_lock);
				printf("Adding job to IO Queue, Thread number: %d\n",(int)thread);
				enqueue(io_ptr,cpu);
				sem_post(&add_io_lock);
				sem_post(&add_io);
			}
			
		}
	}
	pthread_exit(0);
}

int main() {
	queue_init(run_ptr,qSIZE);
	queue_init(io_ptr,qSIZE);
	queue_init(done_ptr,qSIZE);
	job j[jSIZE];
	job *p = j;
	srand(time(NULL));
	for (int i=0; i<jSIZE; i++) {
		init_job(p,i);
		p++;
	}
	p =j;
	for (int i=0; i<jSIZE; i++) {
		enqueue(run_ptr,*p);
		p++;
	}
	void * arg;
	pthread_t cpu[8];
	// pthread_t io[4];
	// pthread_t job_submission[4];
	
	for (int i=0; i<8; i++) {
		int rc = pthread_create(&cpu[i],NULL,cpu_thread,arg);
		assert(rc == 0);
	}
	while (!isEmpty(run_ptr)) {
		
	}
	
	for (int i=0; i<8; i++) {
		int rc = pthread_join(cpu[i],NULL);
		assert(rc==0);
	}
	/*
	for (int i=0; i<4; i++) {
		int rc = pthread_create(&cpu,NULL,io_thread,arg);
		asssert(rc == 0);
	}
	for (int i=0; i<4; i++) {
		int rc = pthread_create(&cpu,NULL,job_submission_thread,arg);
		asssert(rc == 0);
	}
	*/
	return 0;
}
