#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "node.h"

#ifndef LIST_H_   /* Include guard */
#define LIST_H_

typedef struct Queue {
 	int qsize;		//4 bytes
 	node* head; 	//8 bytes
	node* tail;		//8 bytes

	void (*enqueue) (struct Queue*, struct Job*); 	//8 bytes
	void (*destroy) (struct Queue*);				//8 bytes
	job* (*dequeue) (struct Queue*);				//8 bytes
 }queue;
 
int isEmpty(queue *q);

void insert(queue *q,job *j);
 
job *removejob(queue *q);

queue* queue_init();

void queue_delete(queue *q);

#endif 
