obj = distfield.o
bin = distfield
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext -lfreetype $(libgl)

fonts = sans.ttf

sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), Darwin)
	libgl = -framework OpenGL -framework GLUT -lGLEW
else ifeq ($(sys), mingw)
	libgl = -lopengl32 -lglu32 -lfreeglut -lglew32
else
	libgl = -lGL -lGLU -lglut -lGLEW
endif


$(bin): $(obj) $(fonts)
	$(CC) -o $@ $(obj) $(LDFLAGS)

sans.ttf: ../fonts/linux-biolinum.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
