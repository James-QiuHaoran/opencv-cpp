
CXX=g++
CVFLAGS=$(shell pkg-config --cflags opencv)
CVLIBS=$(shell pkg-config --libs opencv)

DEBUGFLAGS = -g -w   #-DTEST 

#GSTREAMERFLAGS=$(shell pkg-config --cflags gstreamer-1.0)
#GSTREAMERLIBS=$(shell pkg-config --libs gstreamer-1.0)

CXXFLAGS   = $(CVFLAGS) $(DEBUGFLAGS)
CXXFLAGS += --std=c++11

OTHERLIBS = 

LIBS = $(CVLIBS) $(OTHERLIBS)

selfdriving: ./src/main.o ./src/fileio.o ./src/utility.o ./src/object_detection.o ./src/geometry.o ./src/routines.o
	$(CXX) $(CXXFLAGS) $^ -o selfdriving $(LIBS)

$(BUILD_DIR)%.o: ./src/%.cpp ./src/%.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	
clean:
	rm selfdriving ./src/*.o
	
	
