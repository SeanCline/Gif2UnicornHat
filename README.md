Gif2UnicornHat
==============

Plays 8x8 gifs on the [Pimoroni UnicornHat](http://shop.pimoroni.com/products/unicorn-hat).

#Installation#

##Install dependencies##

	sudo apt-get install build-essential git libgif-dev libboost-program-options-dev

##Clone and build Gif2UnicornHat##

	cd ~
	git clone --recursive https://github.com/SeanCline/Gif2UnicornHat.git
	cd Gif2UnicornHat
	make dependencies && make


#Usage#
	
Gif2UnicornHat takes a single argument -- an 8x8 gif to play on the UnicornHat:

	sudo ./Gif2UnicornHat examples/loading.gif
