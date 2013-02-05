all updater.c updater.h:
	gcc updater.c -o updater `curl-config --cflags --libs` -g
	