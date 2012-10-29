CC = g++-4.7
# If you have a 64-bit computer, you may want to use this instead.
# CC = gcc -m32
CFLAGS =  -std=c++11 -pedantic -Wall -Werror -Weffc++ -Wextra -Wmain -Wshadow
OPTFLAG = -O2
DEBUGFLAG = -g

# The CC and H files
CCFILES = socket.cc tcp_socket.cc waiter.cc
HFILES = socket.h tcp_socket.h

PROGRAM = waiter

run: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM): $(CCFILES)
	$(CC) $(CFLAGS) $(OPTFLAG) $(CCFILES) -o $@

clean:
	rm -rf $(PROGRAM) *.gch *.gz *.zip

tarball:
	tar -cvzf $(PROGRAM).tar.gz *.cc *.h Makefile

zip:
	zip $(PROGRAM).zip *.cc *.h Makefile
