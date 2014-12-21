Gif2UnicornHat
==============

Plays 8x8 gifs on the [Pimoroni UnicornHat](http://shop.pimoroni.com/products/unicorn-hat).

#Installation#

##Install dependencies##
	sudo apt-get install build-essential git libgif-dev

##Clone and build Gif2UnicornHat##

	cd ~
	git clone https://github.com/SeanCline/Gif2UnicornHat.git
	cd Gif2UnicornHat
	git submodule init
	git submodule update
	make dependencies && make


#Usage#
	
Gif2UnicornHat takes a single argument -- an 8x8 gif to play on the UnicornHat:

	sudo ./Gif2UnicornHat examples/loading.gif
