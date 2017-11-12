#include <stdio.h>
#include <stdlib.h>
#include <job.h>

#ifndef LIST_H_   /* Include guard */
#define LIST_H_

typedef struct Node {
	job* j;
	struct Node* next;
}node;


typedef struct Queue {
 	int qsize;	
 	node* head; 
	node* tail;
 }queue;
 
int isEmpty(queue *q);

void enqueue(queue *q,job *j);
 
job *dequeue(queue *q);

void queue_init(queue *q);

void queue_delete(queue *q);

#endif 
