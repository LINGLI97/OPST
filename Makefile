# Makefile
SDSLPATH = ~
CXX = g++
CXXFLAGS = -std=c++17 -g -I$(SDSLPATH)/include -DUNORDERED_DENSE -msse3 -O3 -fomit-frame-pointer -funroll-loops -DCHECK -DINT2PS
#-DVERBOSE
#-DCHECK
#-DINT2PS

#CXXFLAGS = -std=c++11 -O3 -g

LDFLAGS = -L$(SDSLPATH)/lib
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


