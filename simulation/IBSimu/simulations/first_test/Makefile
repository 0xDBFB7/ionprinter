CC = g++
LDFLAGS = `pkg-config --libs ibsimu-1.0.6`
CXXFLAGS = -Wall -g `pkg-config --cflags ibsimu-1.0.6`

plasmacyl: plasmacyl.o
	$(CC) -o plasmacyl plasmacyl.o $(LDFLAGS)

plasmacyl.o: plasmacyl.cpp
	$(CC) -c -o plasmacyl.o plasmacyl.cpp $(CXXFLAGS)

clean:
	$(RM) *~ *.o plasmacyl
