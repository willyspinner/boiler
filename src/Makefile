CC=g++
FLAGS=-std=c++11 -Wall -Wextra -stdlib=libc++ -O3
default: boiler


# main boiler file.
boiler: boiler.o input.o printutils.o configfile.o errors.o
	$(CC) $(FLAGS) input.o printutils.o configfile.o boiler.o errors.o -o boiler

# object files:
boiler.o: boiler.cc
	$(CC) $(FLAGS) -c boiler.cc -o boiler.o

input.o: input.h input.cc
	$(CC) $(FLAGS) -c input.cc -o input.o

printutils.o: printutils.h printutils.cc
	$(CC) $(FLAGS) -c printutils.cc -o printutils.o

configfile.o: configfile.cc configfile.h
	$(CC) $(FLAGS) -c configfile.cc -o configfile.o

errors.o: errors.cc errors.h
	$(CC) $(FLAGS) -c errors.cc -o errors.o
# make install:
install:
	echo "TODO: install"

clean: 
	rm *.o boiler

