#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue.h>

#define qSIZE 4
#define jSIZE 4

int job_counter;
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
	
	return 0;
}
