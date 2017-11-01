
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>


void queue_init(queue *q,int capacity) {
	q->jobqueue = (job*)calloc(size,sizeof(job));
	q->qcapacity = capacity;
	q->head = 0;
	q->tail = capacity-1;
	q->qsize = 0;
}

void enqueue(queue *q,job j) {
	if (isFull(q) { return; } 
	q->tail = (q->tail + 1)%queue->qsize;
	q->jobqueue[q->tail] = &j;
	q->qsize++;
	    // first job (3+1)%4 = 0
}

job* dequeue(queue *q) {
	if (isEmpty(q) { return NULL;}
	job* ret = queue->jobqueue[queue->head];
	queue->head = (queue->head+1)%queue->qcapacity;
	queue->qsize--;
	return ret;
}

int isEmpty(queue *q){
	return (queue->qsize ==0);	
}	    

int isFull(queue *q){
	return (queue->qsize ==queue->qcapacity);	
}	    


