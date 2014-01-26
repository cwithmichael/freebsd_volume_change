all:
	gcc -g `pkg-config --cflags --libs gtk+-2.0` mymixer.c -o mix
	mkdir /usr/local/share/freebsd_mix/
	cp sound.png /usr/local/share/freebsd_mix/
	mv mix /usr/local/bin/ 


