#include "threads.h"


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


void schedule_init(void) {
	counter = 1;
	job_counter = 0;
	stop = 0;
	
	sem_init(&ready_empty,0,16);
	sem_init(&ready_full,0,0);
	sem_init(&ready_lock,0,1);

	sem_init(&io_empty,0,8);
	sem_init(&io_full,0,0);
	sem_init(&io_lock,0,1);

	sem_init(&finished_empty,0,8);
	sem_init(&finished_full,0,0);
	sem_init(&finished_lock,0,1);
	
	sem_init(&counter_lock,0,1);
	sem_init(&job_counter_lock,0,1);
	
	run_ptr = queue_init(run_ptr);
	io_ptr = queue_init(io_ptr);
	done_ptr = queue_init(done_ptr);
}

// Start job_submission, cpu and io threads
void schedule_thread_start(void) {
	job_submission = malloc(4*sizeof(pthread_t));
	for(long i = 0 ; i < 4; i++){
		int rc = pthread_create(&job_submission[i], NULL,job_submission_thread, (void *)i);
		assert(rc == 0);
	}
	// CPU thread initialization
	cpu = malloc(8*sizeof(pthread_t));
	for (long i=0; i<8; i++) {
		int rc = pthread_create(&cpu[i],NULL,cpu_thread, (void *)i);
		assert(rc == 0);
	}
	// IO thread initialization
	io = malloc(4*sizeof(pthread_t));
	for (long i=0; i<4; i++) {
		int rc = pthread_create(&io[i],NULL,io_thread, (void *)i);
		assert(rc == 0); // Error is rc is not 0
	}
}

// Join the job_submission, cpu and io threads
void schedule_thread_join(void) {
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
		assert(rc==0); // Error is rc is not 0
	}
}

void schedule_destroy(void) {
	sem_destroy(&ready_lock);
	sem_destroy(&io_lock);
	sem_destroy(&finished_lock);
	sem_destroy(&counter_lock);
	sem_destroy(&job_counter_lock);
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
}

void* job_submission_thread(void* arg){
	int thread = (long) arg;
	int t;
	while(stop!=1){
		// Producer
    sem_wait(&counter_lock);
		job *j = create_job(counter);
    counter++;
		sem_post(&counter_lock);

		sem_wait(&ready_lock);
		printf("Job: #%02d -> Run  Queue, JOB Thread: #%d\n",j->job_id, thread);
		run_ptr->enqueue(run_ptr,j);
		sem_post(&ready_lock);
		// Consumer
		t = 0;
		while (t!=1){
		    if(!isEmpty(done_ptr)){
					sem_wait(&finished_lock);
					job *ja= done_ptr->dequeue(done_ptr);
					sem_post(&finished_lock);
					if (ja == NULL) {
							sleep(2);
							continue;
					}
					sem_wait(&job_counter_lock);
					job_counter++;
					sem_post(&job_counter_lock);
					printf("Job: #%02d -> Job  Freed, JOB Thread: #%d\n",ja->job_id, thread);
					ja->destroy(ja);
		    }
		    t = wait(4);
		}
	}
	printf("JOB Thread: #%d is exiting\n", thread);
	pthread_exit(0);
}

void * cpu_thread(void * arg) { 
	int thread = (long) arg;
	while (stop!=1) {
		if (!isEmpty(run_ptr)) {
			sem_wait(&ready_lock);
			job *cpu = run_ptr->dequeue(run_ptr);
			sem_post(&ready_lock);
			if (cpu == NULL) {
				sleep(2);
				continue;
			}
			
			if (cpu->task_type == 0) {
				cpu->complete(cpu);
				cpu->next(cpu);
			}

			if (cpu->is_completed == 1) {
				printf("Job: #%02d -> Done Queue, CPU Thread: #%d\n",cpu->job_id,thread);
				sem_wait(&finished_lock);
				done_ptr->enqueue(done_ptr,cpu);
				sem_post(&finished_lock);
			} 
			else if (cpu->task_type == 0) {
				printf("Job: #%02d -> Run  Queue, CPU Thread: #%d\n",cpu->job_id, thread);
				sem_wait(&ready_lock);
				run_ptr->enqueue(run_ptr,cpu);
				sem_post(&ready_lock);
			} 
			else {
				printf("Job: #%02d -> IO   Queue, CPU Thread: #%d\n",cpu->job_id,thread);
				sem_wait(&io_lock);
				io_ptr->enqueue(io_ptr,cpu);
				sem_post(&io_lock);
			}
		}
	}
	printf("CPU Thread: #%d is exiting\n",thread);
	pthread_exit(0);
}

void * io_thread(void* arg) { 
	int thread = (long) arg;
	while (stop != 1) {
		if (!isEmpty(io_ptr)) {
			sem_wait(&io_lock);
			job *io = io_ptr->dequeue(io_ptr);
			sem_post(&io_lock);
			if (io == NULL) {
				sleep(2);
				continue;
			}

			if (io->task_type == 1){
				io->complete(io);
				io->next(io);
			}		
			
			if (io->is_completed == 1) {
				printf("Job: #%02d -> Done Queue, IO  Thread: #%d\n", io->job_id, thread);
				sem_wait(&finished_lock);
				done_ptr->enqueue(done_ptr,io);
				sem_post(&finished_lock);

			} else if (io->task_type == 0) {
				printf("Job: #%02d -> Run  Queue, IO  Thread: #%d\n", io->job_id,thread);
				sem_wait(&ready_lock);
				run_ptr->enqueue(run_ptr,io);
				sem_post(&ready_lock);

			} else {
				printf("Job: #%02d -> IO   Queue, IO  Thread: #%d\n",io->job_id,thread);
				sem_wait(&io_lock);
				io_ptr->enqueue(io_ptr,io);
				sem_post(&io_lock);
			}	
		}
	}
	printf("IO Thread: #%d is exiting\n",thread);
	pthread_exit(0);
}