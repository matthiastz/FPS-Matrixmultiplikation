CC = gcc
CFLAGS = -Wall -std=c99 -lm -mavx
LFLAGS = -lcblas
SRCDIR = src
INCLUDEDIR = src/include



debug:clean
	$(CC) $(CFLAGS) -g -Wextra -I$(INCLUDEDIR) $(SRCDIR)/*.c $(LFLAGS)
default:clean
	$(CC) $(CFLAGS) -I$(INCLUDEDIR)  $(SRCDIR)/*.c $(LFLAGS)
clean:
	rm -vfr *~ a.out



