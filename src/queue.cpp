
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int QUEUE::isEmpty()
{
    return (qsize == 0);
}

QUEUE::QUEUE()
    : qsize(0), head(NULL), tail(NULL)
{
}

JOB *QUEUE::dequeue_job()
{
    if (isEmpty())
    {
        return NULL;
    }
    JOB *j;
    NODE *tmp = head;
    head = head->next;
    j = tmp->j;
    delete tmp;
    qsize--;
    return j;
}

QUEUE::~QUEUE()
{
    while (qsize > 0)
    {
        JOB *j = dequeue_job();
        delete j;
    }
}

void QUEUE::enqueue_job(int id)
{
    if (qsize == 0)
    {
        head = new NODE(id);
        tail = head;
    }
    else
    {
        NODE *temp = new NODE(id);
        tail->next = temp;
        tail = temp;
    }
    qsize++;
}

void QUEUE::requeue_job(JOB *job)
{
    if (qsize == 0)
    {
        head = new NODE(job);
        tail = head;
    }
    else
    {
        NODE *temp = new NODE(job);
        tail->next = temp;
        tail = temp;
    }
    qsize++;
}

void QUEUE::print_queue()
{
    NODE *current = head;
    while (current != NULL)
    {
        printf("");
    }
}
