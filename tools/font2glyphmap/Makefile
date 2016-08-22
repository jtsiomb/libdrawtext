PREFIX = /usr/local

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
bin = font2glyphmap

CC = gcc
CFLAGS = -pedantic -Wall -g -I../../src -I/usr/local/include
LDFLAGS = -L. -L/usr/local/lib -ldrawtext

ifeq ($(shell uname -s), Darwin)
	lib_so = libdrawtext.dylib
else
	lib_so = libdrawtext.so.0.3
endif

$(bin): $(obj) $(lib_so) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(lib_so): ../../$(lib_so)
	rm -f $@
	ln -s $< $@

.PHONY: clean
clean:
	rm -f $(obj) $(bin) $(lib_so)

.PHONY: install
install: $(bin)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(bin) $(DESTDIR)$(PREFIX)/bin/$(bin)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(bin)
