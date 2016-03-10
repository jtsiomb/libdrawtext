obj = simple.o
bin = simple
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext $(libgl)

font = serif.ttf

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(font): ../fonts/linux-libertine.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
