#include <stdio.h>
#include <stdlib.h>
#include "job.h"

#ifndef LIST_N_   /* Include guard */
#define LIST_N_

typedef struct Node {
	job* j;
	struct Node* next;

	job* (*destroy) (struct Node*);

}node;

node* init_node(job *j);
job* node_destroy(node *n);


#endif