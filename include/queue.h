#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "node.h"

struct QUEUE
{
    int qsize;  //Current Size of the queue
    NODE *head; //Head of the queue
    NODE *tail; //Tail of the queue

    QUEUE();
    ~QUEUE();

    int isEmpty();

    void enqueue_job(int id);
    void requeue_job(JOB *job);

    JOB *dequeue_job();

    void print_queue();
};
