obj = raster.o
bin = raster
root = ../..

CFLAGS = -pedantic -Wall -g -I$(root)/src -I/usr/local/include
LDFLAGS = -L$(root) -Wl,-rpath=$(root) -ldrawtext

font = serif.ttf

$(bin): $(obj) $(lib_so) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(font): ../fonts/linux-libertine.ttf
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin)
