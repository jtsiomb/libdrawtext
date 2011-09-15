obj = unicode.o
bin = unicode

CC = gcc
CFLAGS = -pedantic -Wall -g -I../../src
LDFLAGS = -L. -ldrawtext $(libgl)

lib_so = libdrawtext.so
fonts = serif.ttf klingon.ttf

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj) $(lib_so) $(fonts)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(lib_so): ../../libdrawtext.so.0.0
	rm -f $@
	ln -s $< $@

serif.ttf: ../fonts/FreeSerif.ttf
	rm -f $@
	ln -s $< $@

klingon.ttf: ../fonts/klingon.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin) $(lib_so)
