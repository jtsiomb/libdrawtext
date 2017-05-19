obj = simple.o
bin = simple
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext -lfreetype $(libgl)

font = serif.ttf

sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), Darwin)
	libgl = -framework OpenGL -framework GLUT
else ifeq ($(sys), mingw)
	libgl = -lopengl32 -lglu32 -lfreeglut
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
