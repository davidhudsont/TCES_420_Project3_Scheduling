IDIR = ../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR = obj

_DEPS = threads.h queue.h node.h job.h
DEPS = $(patsubst %, $(ODIR)/%, $(_OBJ))

_OBJ = scheduler.o scheduler_threads.o queue.o node.o job.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ main