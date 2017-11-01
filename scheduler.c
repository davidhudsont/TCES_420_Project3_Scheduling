#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue.h>

#define qSIZE 4
#define jSIZE 4

queue run;
queue io;
queue done;
queue *run_ptr = &run;
queue *io_ptr = &io;
queue *done_ptr = &done;

void delay(int time){
	int t = clock();
	while ((float)((clock()-t)/CLOCKS_PER_SEC) <time) {}
}

int main() {
	queue_init(run_ptr,qSIZE);
	job j[jSIZE];
	job *p = j;
	srand(time(NULL));
	for (int i=0; i<jSIZE; i++) {
		init_job(p,i,rand()%5+1,rand()%10+1, 0);
		p++;
	}
	p = j;
	for (int i=0; i<qSIZE; i++) {
		enqueue(run_ptr,*p);
		p++;
	}
	p = j;
	while (!isEmpty(run_ptr)) {
		job x = *dequeue(run_ptr);
		printf("id: %d\n", x.job_id);
	}
	return 0;
}
