


schedule: scheduler.c job.c node.c queue.c
	gcc -o run scheduler.c job.c node.c queue.c -lpthread -std=gnu99 -I.
# main: main.c job.c node.c queue.c
	# gcc -o run main.c job.c node.c queue.c -std=gnu99 -I.
