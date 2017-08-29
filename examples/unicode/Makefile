obj = unicode.o
bin = unicode
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext -lfreetype $(libgl)

fonts = serif.ttf cjk.ttf klingon.ttf

sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), Darwin)
	libgl = -framework OpenGL -framework GLUT
else ifeq ($(sys), mingw)
	libgl = -lopengl32 -lglu32 -lfreeglut
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
