
#include <stdlib.h>
#include <stdio.h>

#define SIZE 4

struct queue {
	int* runqueue;
	
	
};

int enqueue(struct queue run,int x) {
	for (int i=0; i<SIZE; i++) {
		if (run.runqueue[i]==0) {
			run.runqueue[i] = x;
			return x;
		}
		else {
			
		}
	}
	return 0;
}
int dequeue(struct queue run) {
	int first = run.runqueue[0];
	for (int i=0; i<SIZE; i++) {
		if (i>=SIZE-1) {
			run.runqueue[i] = 0;
			break;
		}
		run.runqueue[i] = run.runqueue[i+1];
	}
	return first;
}


int main() {
	struct queue run;
	
	run.runqueue = (int*)calloc(SIZE,sizeof(int));	
	enqueue(run,5);
	enqueue(run,3);
	enqueue(run,2);
	enqueue(run,1);
	printf("\n");
	for (int i=0; i<SIZE; i++) {
		printf("%d\n",run.runqueue[i]);
	}
	dequeue(run);
	printf("\n");
	for (int i=0; i<SIZE; i++) {
		printf("%d\n",run.runqueue[i]);
	}
	
	return 0;
	
	
	
	
}