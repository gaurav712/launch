CC="gcc"
CFLAGS="-Wall"

all:
	$(CC) -o launch $(CFLAGS) launch.c

clean:
	rm -f launch

install: all
	cp launch /usr/local/bin/launch

uninstall:
	rm -f /usr/local/bin/launch
