CC = g++
LDFLAGS = `pkg-config --libs ibsimu-1.0.6 umfpack` `Magick++-config --ldflags --libs`
CXXFLAGS = -Wall -g `pkg-config --cflags ibsimu-1.0.6 umfpack` -std=c++17 `Magick++-config --cxxflags --cppflags`

plasmacyl: einzel.o
	$(CC) -o einzel einzel.o $(LDFLAGS)

plasmacyl.o: einzel.cpp
	$(CC) -c -o einzel.o einzel.cpp $(CXXFLAGS)

clean:
	$(RM) *~ *.o einzel
