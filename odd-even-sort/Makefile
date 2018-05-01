IDIR=.

CC=mpicc
CFLAGS=-lm -Wall -Wextra -Wno-unused-command-line-argument -std=gnu99

LIBS = util
LIBS_O = $(addsuffix .o, $(LIBS))

ODD_EVEN_MIN_OBJS=$(IDIR)/odd-even-min.c $(LIBS_O)
ODD_EVEN_OBJS=$(IDIR)/odd-even.c $(LIBS_O)

.DEFAULT_GOAL := all
.PHONY: clean

ALL=odd-even odd-even-min
all: $(ALL)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

odd-even-min: $(ODD_EVEN_MIN_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

odd-even: $(ODD_EVEN_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(IDIR)/*.o $(IDIR)/**/*.o
	rm -f $(ALL)
