#include <stdio.h>
#include <stdlib.h>
#include <job.h>

#ifndef LIST_H_   /* Include guard */
#define LIST_H_

typedef struct Queue {
 	int qsize,qcapacity;	
 	int head,tail;
 	job* jobqueue; 
 }queue;
 
int isEmpty(queue *q);

int isFull(queue *q);
 
 void enqueue(queue *q,job j);
 
 job dequeue(queue *q);

 void queue_init(queue *q,int size);

#endif 
