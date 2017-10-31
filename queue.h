#include <stdio.h>
#include <stdlib.h>
#include <job.h>

#ifndef LIST_H_   /* Include guard */
#define LIST_H_

typedef struct node_tag {
   job* data;
   struct node_tag *next;
} Node;

typedef struct Queue {
	Node* head;
	Node* end;
}queue;


void enqueue(queue *q,job j);

job* dequeue(queue *q);

void queue_init(queue *q,int size);


#endif 