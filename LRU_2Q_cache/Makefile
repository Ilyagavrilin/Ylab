#files with source code
SOURCE= main.cpp
OBJECTS=$(SOURCE:.cpp=.o)
#resulting file
EXECUTABLE=cache
#compilation options
DBGFLAGS = -Wall -Wfloat-equal -Wundef -DDEBUG


all: $(SOURCE) $(EXECUTABLE) clean

debug:
	$(CXX) $(SOURCE) $(DBGFLAGS)  -o $(EXECUTABLE)


build_obj: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

.c.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean_all:
	rm -rf *.o $(EXECUTABLE)

clean:
	rm -rf *.o 