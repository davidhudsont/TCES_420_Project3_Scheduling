
// Include file

typedef struct Queue {
	int qsize;	
	int head,tail;
	job* jobqueue; 
}queue;


void enqueue(queue *q,job j);

job* dequeue(queue *q);

int queue_size(queue *q);

void queue_init(queue *q,int size);
