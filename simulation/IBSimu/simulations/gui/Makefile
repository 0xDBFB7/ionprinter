CC = g++
LDFLAGS = `pkg-config --libs ibsimu-1.0.6 umfpack cairo plplot` `Magick++-config --ldflags --libs` `fltk-config --use-cairo --ldflags` -lfltk -lplplot -lplplotd -lplplotcxxd -lmgl -lfltk_images -lmgl-fltk
CXXFLAGS = -Wall -g `pkg-config --cflags ibsimu-1.0.6 umfpack plplot cairo` -std=c++17 -DFLTK_HAVE_CAIRO `Magick++-config --cxxflags --cppflags` `fltk-config --use-cairo --cxxflags` -lfltk -lplplot -lfltk_images

plasmacyl: einzel.o
	$(CC) -o einzel einzel.o $(LDFLAGS)

plasmacyl.o: einzel.cpp
	$(CC) -c -o einzel.o einzel.cpp $(CXXFLAGS)

clean:
	$(RM) *~ *.o einzel
