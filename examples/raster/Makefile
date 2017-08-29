obj = raster.o
bin = raster
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src -I/usr/local/include
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext -lfreetype

font = serif.ttf

# OpenGL is not used by this example at all, but we need to link with it
# on windows when we're linking with a version of drawtext compiled with
# OpenGL enabled.
sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	LDFLAGS += -lopengl32 -lglu32 -lwinmm
endif


$(bin): $(obj) $(lib_so) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(font): ../fonts/linux-libertine.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
