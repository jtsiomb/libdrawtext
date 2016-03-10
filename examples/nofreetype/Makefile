obj = simple-noft.o
bin = simple-noft
root = ../..

CC = gcc
CFLAGS = -pedantic -Wall -g -I$(root)/src -I/usr/local/include
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext-noft $(libgl)

font = serif.ttf

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
