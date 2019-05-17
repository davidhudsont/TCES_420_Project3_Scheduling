IDIR = ../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR = obj

LIBS = -lpthread

_DEPS = threads.h queue.h node.h job.h
DEPS = $(patsubst %, $(ODIR)/%, $(_OBJ))

_OBJ = scheduler.o scheduler_threads.o queue.o node.o job.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ main