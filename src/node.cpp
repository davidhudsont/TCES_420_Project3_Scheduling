#include <stdlib.h>
#include <stdio.h>
#include "node.h"

NODE::NODE(int id)
{
    this->j = new JOB(id);
    this->next = NULL;
}

NODE::NODE(JOB *job)
{
    this->j = job;
    this->next = NULL;
}