#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "node.h"

#ifndef __QUEUE__H__   /* Include guard */
#define __QUEUE__H__

typedef struct queue {
 	int qsize;		//Current Size of the queue
 	NODE* head; 	//Head of the queue
	NODE* tail;		//Tail of the queue

 } QUEUE;
 
int isEmpty(QUEUE* q);

void enqueue_job(QUEUE* q, JOB* j);
 
JOB *dequeue_job(QUEUE* q);

QUEUE* queue_init();

void queue_delete(QUEUE* q);

void print_queue(QUEUE* q);

#endif 
