CXXFLAGS=-g -m64 -O2 -Wall $(shell root-config --cflags ) 
LIBS=-g -m64  $(shell root-config --libs) -lMathMore  -lGenVector

.PHONY: all clean

all: makePlots

makePlots :	main.o makePlots.o 
		g++ $^ -o $@ $(LIBS)

main.o :	main.cc  makePlots.h
		g++ -c $(CXXFLAGS) $< -o $@

makePlots.o :    makePlots.cc   makePlots.h  
		g++ -c $(CXXFLAGS) $< -o $@

clean:	
	rm -f ./*.o 
	rm -f makePlots
