fonts = fonts/linux-libertine.ttf fonts/klingon.ttf fonts/sazanami-mincho.ttf

.PHONY: all
all: $(fonts)
	$(MAKE) -C simple
	$(MAKE) -C unicode
	$(MAKE) -C raster
	$(MAKE) -C nofreetype
	$(MAKE) -C distfield

fonts/linux-libertine.ttf:
	mkdir -p fonts
	wget http://downloads.sourceforge.net/project/linuxlibertine/linuxlibertine/5.1.3-2/LinLibertineTTF_5.1.3_2011_06_21.tgz
	mkdir -p linlibertine
	cd linlibertine; tar xzvf ../LinLibertineTTF_5.1.3_2011_06_21.tgz
	rm -f LinLibertineTTF_5.1.3_2011_06_21.tgz
	cp linlibertine/LinLibertine_R.ttf $@
	cp linlibertine/LinBiolinum_R.ttf fonts/linux-biolinum.ttf
	rm -rf linlibertine

fonts/sazanami-mincho.ttf:
	mkdir -p fonts
	wget -O sazanami-20040629.tar.bz2 'http://sourceforge.jp/frs/redir.php?m=iij&f=/efont/10087/sazanami-20040629.tar.bz2'
	tar xjvf sazanami-20040629.tar.bz2
	rm -f sazanami-20040629.tar.bz2
	cp sazanami-20040629/sazanami-mincho.ttf $@
	rm -rf sazanami-20040629

fonts/klingon.ttf:
	mkdir -p fonts
	wget https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Components.PostAttachments/00/10/42/00/93/pIqaD%20Font.zip
	unzip -o 'pIqaD Font.zip'
	rm -f 'pIqaD Font.zip'
	cp 'pIqaD Font/pIqaD.ttf' $@
	rm -rf 'pIqaD Font'
