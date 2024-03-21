# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -I ./libsdsl/include/ -msse3 -O3 -fomit-frame-pointer -funroll-loops
#-DVISUALIZATION
# -DUNORDERED_DENSE
 #-DVISUALIZATION
#-DUNORDERED_DENSE -msse3 -O3 -fomit-frame-pointer -funroll-loops
# 
#-DVERBOSE
#-DCHECK


#CXXFLAGS = -std=c++11 -O3 -g

LDFLAGS = -L ./libsdsl/lib/
LIBS = -lsdsl -ldivsufsort -ldivsufsort64


# CXXFLAGS += -I/path/to/sdsl/include
# LDFLAGS += -L/path/to/sdsl/lib

SOURCES = main.cpp node.cpp OPST.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean


