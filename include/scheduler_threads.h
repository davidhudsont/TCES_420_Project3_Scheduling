#pragma once

int wait(int time);

void stopScheduler();

int getCounter();
int getJobCounter();

void schedule_init(void);

void schedule_thread_start(void);

void schedule_thread_join(void);
