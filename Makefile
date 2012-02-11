# Makefile for showimage+
# Last edited: <29 Oct 2003 19:25:27 m. boeh>

VERSION="\"15\""
override CFLAGS +=  `sdl-config --cflags` -DVERSION=$(VERSION) -O2 -Wall -ansi -pedantic
override LFLAGS +=  `sdl-config --libs` -lSDL_image
LOADER=sdl

all: showimage+

static: all

showimage+: showimageplus.o images.o imagelist.o $(LOADER)_load_image.o
	$(CC) $+ $(LFLAGS) -o $@

.c.o:
	$(CC) -c $< $(CFLAGS) -o $@

install: showimage+
	cp showimage+ ~/bin/
clean: clean-exec clean-obj

clean-exec:
	-rm showimage+

clean-obj:
	-rm *.o
