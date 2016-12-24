CC = gcc
CFLAGS = -Wall -std=c99 -lm -mavx
SRCDIR = src
INCLUDEDIR = src/include

debug:clean
	$(CC) $(CFLAGS) -g -Wextra -I$(INCLUDEDIR) $(SRCDIR)/*.c
default:clean
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(SRCDIR)/*.c
clean:
	rm -vfr *~ a.out
