obj = simple.o
bin = simple

CC = gcc
CFLAGS = -pedantic -Wall -g -I../../src
LDFLAGS = -L. -ldrawtext $(libgl)

lib_so = libdrawtext.so
font = serif.ttf

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj) $(lib_so) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(lib_so): ../../libdrawtext.so.0.0
	rm -f $@
	ln -s $< $@

$(font): ../fonts/FreeSerif.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin) $(lib_so)
