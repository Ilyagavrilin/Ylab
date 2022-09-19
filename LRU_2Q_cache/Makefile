

#files with source code
SOURCE= main.cpp
OBJECTS=$(SOURCE:.cpp=.o)
#resulting file
EXECUTABLE=cache
#compilation options
COMPILER=g++
CPPFLAGS= -Wall -Wfloat-equal -Wundef -std=c++20
DBGFLAGS = -Wall -Wfloat-equal -Wundef -std=c++20 -DDEBUG


all: $(SOURCE) $(EXECUTABLE) clean

debug:
	$(COMPILER) $(SOURCE) $(DBGFLAGS)  -o $(EXECUTABLE)


build_obj: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@

.c.o:
	$(COMPILER) -c $(CPPFLAGS) $< -o $@

clean_all:
	rm -rf *.o $(EXECUTABLE)

clean:
	rm -rf *.o 