#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue.h>
#include <assert.h>
//#include <pthread.h>
//#include <unistd.h>
#define qSIZE 100

// unsigned int sleep(unsigned int seconds);

queue run;
queue io;
queue done;
queue *run_ptr = &run;
queue *io_ptr = &io;
queue *done_ptr = &done;

int main() {
	void * arg;
	// pthread_t cpu[8];
	// pthread_t io[4];
	// pthread_t job_submission[4];
	/*
	for (int i=0; i<8; i++) {
		int rc = pthread_create(&cpu,NULL,cpu_thread,arg);
		asssert(rc == 0);
	}
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
