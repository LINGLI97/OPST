# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -g -I~/include
#CXXFLAGS = -std=c++11 -O3 -g

LDFLAGS = -L~/lib
LIBS = -lsdsl -ldivsufsort -ldivsufsort64

# 如果SDSL安装在非标准路径，你需要指定它们的位置
# CXXFLAGS += -I/path/to/sdsl/include
# LDFLAGS += -L/path/to/sdsl/lib

# 项目的源文件
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


