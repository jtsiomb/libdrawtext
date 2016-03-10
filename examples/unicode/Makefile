obj = unicode.o
bin = unicode
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext $(libgl)

fonts = serif.ttf cjk.ttf klingon.ttf

ifeq ($(shell uname -s), Darwin)
	libgl = -framework OpenGL -framework GLUT
else
	libgl = -lGL -lGLU -lglut
endif

$(bin): $(obj) $(fonts)
	$(CC) -o $@ $(obj) $(LDFLAGS)

serif.ttf: ../fonts/linux-libertine.ttf
	rm -f $@
	ln -s $< $@

cjk.ttf: ../fonts/sazanami-mincho.ttf
	rm -f $@
	ln -s $< $@

klingon.ttf: ../fonts/klingon.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
