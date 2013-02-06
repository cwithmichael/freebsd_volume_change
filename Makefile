all:
	gcc -g `pkg-config --cflags --libs gtk+-2.0` mymixer.c -o mix
	mv mix /usr/local/bin/ 


