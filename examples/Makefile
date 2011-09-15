fonts = fonts/FreeSerif.ttf fonts/klingon.ttf

.PHONY: all
all: $(fonts)
	cd simple; $(MAKE)

fonts/FreeSerif.ttf:
	mkdir -p fonts
	wget http://ftp.gnu.org/gnu/freefont/freefont-ttf-20100919.tar.gz
	tar xzvf freefont-ttf-20100919.tar.gz
	rm -f freefont-ttf-20100919.tar.gz
	cp freefont-20100919/FreeSerif.ttf $@

fonts/klingon.ttf:
	mkdir -p fonts
	wget http://blogs.msdn.com/cfs-file.ashx/__key/communityserver-components-postattachments/00-00-53-45-33/tlh_2D00_pIqaD_2D00_US.zip
	unzip -o tlh_2D00_pIqaD_2D00_US.zip
	rm -f tlh_2D00_pIqaD_2D00_US.zip
	cp tlh-pIqaD-US/pIqaD.ttf $@
