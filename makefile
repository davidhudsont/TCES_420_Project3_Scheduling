


main: scheduler.o queue.o node.o job.o 
	gcc -o main scheduler.o queue.o node.o job.o
	
schedule.o : queue.h
	gcc -c schedule.c -lpthread
	
queue.o: queue.h
	gcc -c queue.c
	
node.o: node.h
	gcc -c node.c
	
job.o: job.h
	gcc -c job.c

clean:
	rm *.o main