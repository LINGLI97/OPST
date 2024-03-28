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

SOURCES = main.cpp node.cpp OPST.cpp baseline.cpp utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)





#Test random instances for two different methods

TEST_SOURCES = Test.cpp node.cpp OPST.cpp utils.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
TEST_TARGET = run_tests


test: $(TEST_TARGET)


$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean_test:
	rm -f $(TEST_TARGET) $(TEST_OBJECTS)








Ratio_SOURCES = exhaustive.cpp node.cpp OPST.cpp
Ratio_OBJECTS = $(Ratio_SOURCES:.cpp=.o)
Ratio_TARGET = run_ratio


ratio: $(Ratio_TARGET)

$(Ratio_TARGET): $(Ratio_OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean_ratio:
	rm -f $(Ratio_TARGET) $(Ratio_OBJECTS)








.PHONY: all clean




