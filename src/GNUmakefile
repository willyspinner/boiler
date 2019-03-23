CXX=g++
FLAGS=-std=c++11 -Wall -Wextra -O3
default: boiler


# main boiler file.
boiler: boiler.o input.o printutils.o configfile.o errors.o
	$(CXX) $(FLAGS) input.o printutils.o configfile.o boiler.o errors.o -o boiler

# object files:
boiler.o: boiler.cc
	$(CXX) $(FLAGS) -c boiler.cc -o boiler.o

input.o: input.h input.cc
	$(CXX) $(FLAGS) -c input.cc -o input.o

printutils.o: printutils.h printutils.cc
	$(CXX) $(FLAGS) -c printutils.cc -o printutils.o

configfile.o: configfile.cc configfile.h
	$(CXX) $(FLAGS) -c configfile.cc -o configfile.o

errors.o: errors.cc errors.h
	$(CXX) $(FLAGS) -c errors.cc -o errors.o
# make install:
install:
	echo "TODO: install"

clean: 
	rm *.o boiler

