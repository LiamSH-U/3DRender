CXX = g++
CXXFLAGS = -Wall -Wextra

PROGRAM = saveimg

saveimg: main.o
	$(CXX) $(CXXFLAGS) main.o utilities.hpp -o $(PROGRAM)

debug: main_dbg.o
	$(CXX) $(CXXFLAGS) -g3 main_dbg.o utilities.hpp -o $(PROGRAM)_dbg

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

main_dbg.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -g3 main.cpp -o main_dbg.o

topdowngif:
	convert -delay 5 -loop 0 topdown*.ppm topdown.gif

playerviewgif:
	convert -delay 5 -loop - playerview*.ppm playerview.gif

clean:
	rm -f *.o $(PROGRAM) $(PROGRAM)_dbg *.ppm *.gif