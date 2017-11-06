schedule: scheduler.c queue.c queue.h job.c job.h
	gcc -std=c99 -o run scheduler.c queue.c job.c -I.