obj = simple.o
bin = simple
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath,$(root) $(lib) $(libgl)

font = serif.ttf

sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), Darwin)
	lib = -ldrawtext
	libgl = -framework OpenGL -framework GLUT
else ifeq ($(sys), mingw)
	lib = -ldrawtext -lfreetype
	libgl = -lopengl32 -lglu32 -lfreeglut
else
	lib = -ldrawtext
	libgl = -lGL -lGLU -lglut -lm
endif

$(bin): $(obj) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(font): ../fonts/linux-libertine.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
