


schedule: scheduler.c queue.c job.c 
	gcc -o run scheduler.c queue.c job.c -lpthread -std=gnu99 -I.
