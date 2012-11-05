# Point this to your g++ binary
CC = g++-4.7
CFLAGS = -std=c++11 -pedantic -Wall -Werror -Weffc++ -Wextra -Wmain -Wshadow

OFILES = socket.o tcp_socket.o http.o

install: libwaiter.so waiter.h
	cp libwaiter.so /usr/local/lib
	cp *.h /usr/local/include

socket.o: socket.h socket.cc
	$(CC) $(CFLAGS) -fPIC -c socket.cc

tcp_socket.o: tcp_socket.h tcp_socket.cc socket.o
	$(CC) $(CFLAGS) -fPIC -c tcp_socket.cc 

http.o: http.h http.cc tcp_socket.o
	$(CC) $(CFLAGS) -fPIC -c http.cc

libwaiter.so: $(OFILES) waiter.h
	$(CC) $(CFLAGS) -fPIC -shared $(OFILES) waiter.cc -o libwaiter.so

uninstall:
	rm /usr/local/lib/libwaiter.so /usr/local/include/waiter.h \
		 /usr/local/include/socket.h /usr/local/include/tcp_socket.h \
		 /usr/local/include/http.h

clean:
	rm *.o *.so
