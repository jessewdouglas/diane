CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic

diane: main.o
	$(CC) -o diane main.o -lsqlite3

install: diane
	cp diane /usr/local/bin

clean:
	rm -f *.o diane

uninstall:
	rm -f /usr/local/bin/diane
