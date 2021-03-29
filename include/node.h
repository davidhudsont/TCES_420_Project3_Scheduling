#pragma once

#include "job.h"

struct NODE
{
    JOB *j;
    NODE *next;
    NODE(JOB *job);
    NODE(int id);
};
