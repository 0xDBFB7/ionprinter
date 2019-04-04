CC = g++
LDFLAGS = `pkg-config --libs ibsimu-1.0.6 umfpack cairo plplot` `pkg-config python-3.5 --libs`
CXXFLAGS = -Wall -g `pkg-config --cflags ibsimu-1.0.6 umfpack plplot cairo` -std=c++17 `pkg-config python-3.5  --cflags`

plasmacyl: einzel.o
	$(CC) -o einzel einzel.o $(LDFLAGS)

plasmacyl.o: einzel.cpp
	$(CC) -c -o einzel.o einzel.cpp $(CXXFLAGS)

clean:
	$(RM) *~ *.o einzel
