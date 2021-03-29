#pragma once

#include "job.h"
#include "node.h"

class QUEUE
{
private:
    int qsize;  //Current Size of the queue
    NODE *head; //Head of the queue
    NODE *tail; //Tail of the queue
public:
    QUEUE();
    ~QUEUE();

    int isEmpty();

    void enqueue_job(int id);
    void requeue_job(JOB *job);

    JOB *dequeue_job();

    void print_queue();
};
