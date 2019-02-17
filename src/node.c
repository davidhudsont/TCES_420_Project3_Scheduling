#include <stdlib.h>
#include <stdio.h>
#include "node.h"



node* init_node(job *j) {
	node *n = (node*)malloc(sizeof(node));
	n->j = j;
	n->next= NULL;
	n->destroy = node_destroy;
	return n;
}


job* node_destroy(node *n) {
	job *j = n->j;
	n->j = NULL;
	n->next = NULL;
	free(n);
	return j;
}


