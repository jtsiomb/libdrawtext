fonts = fonts/FreeSerif.ttf

.PHONY: all
all: $(fonts)
	cd simple; $(MAKE)

fonts/FreeSerif.ttf:
	wget http://ftp.gnu.org/gnu/freefont/freefont-ttf-20100919.tar.gz
	tar xzvf freefont-ttf-20100919.tar.gz
	mv freefont-20100919 fonts
