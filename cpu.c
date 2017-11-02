#include <stdio.h>
#include <queue.h>
#include <semaphore.h>

#define qSIZE 100

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
	while () {
		if (!isEmpty(run_ptr)) {
			sem_wait(&sub_run);
			sem_wait(&add_run_lock);
			job cpu = dequeue(run_ptr);
			sem_post(&add_run_lock);
			sem_post(&sub_run);
			delay(cpu.cpu_duration);
			cpu.current_phase++;
			if (cpu.current_phase == cpu.tasks) {
				cpu.is_completed = 1;
				sem_wait(&add_finished);
				sem_wait(&add_finished_lock);
				enqueue(done_ptr,cpu);
			}
			if (cpu.phase[1][cpu.current_phase] == 0) {
				sem_wait(&add_run);
				sem_wait(&add_run_lock);
				equeue(run_ptr,cpu);
				sem_post(&add_run_lock);
				sem_post(&add_run);
			}
			if (cpu.phase[1][cpu.current_phase] == 1) {
				sem_wait(&add_io);
				sem_wait(&add_io_lock);
				equeue(io_ptr,cpu);
				sem_post(&add_io_lock);
				sem_post(&add_io);
			}
			
		}
	}
	return NULL;
}