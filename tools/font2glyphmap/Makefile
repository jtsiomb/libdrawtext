PREFIX = /usr/local

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
bin = font2glyphmap

CFLAGS = -pedantic -Wall -g -I../../src
LDFLAGS = -L../.. -Wl,-rpath=../.. -ldrawtext

sys := $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), Darwin)
	lib_so = libdrawtext.dylib
else ifeq ($(sys), mingw)
	lib_so = libdrawtext.dll
	LDFLAGS += -lopengl32 -lglu32 -lfreetype
else
	lib_so = libdrawtext.so.0.3
endif

$(bin): $(obj) ../../$(lib_so) $(font)
	$(CC) -o $@ $(obj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(bin)

.PHONY: install
install: $(bin)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(bin) $(DESTDIR)$(PREFIX)/bin/$(bin)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(bin)
