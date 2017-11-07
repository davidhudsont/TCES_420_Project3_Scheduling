


schedule: thienscheduler.c queue.c job.c 
	gcc -o run thienscheduler.c queue.c job.c -lpthread -std=gnu99 -I.
