# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -I ./libsdsl/include/ -msse3 -O3 -fomit-frame-pointer -funroll-loops
#-DVISUALIZATION


LDFLAGS = -L ./libsdsl/lib/
LIBS = -lsdsl -ldivsufsort -ldivsufsort64


ConstructionSOURCES = OPST_construction.cpp node.cpp OPST.cpp utils.cpp
ConstructionOBJECTS = $(ConstructionSOURCES:.cpp=.o)
ConstructionTARGET = OPSTConstruction


$(ConstructionTARGET): $(ConstructionOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_OPSTConstruction:
	rm -f $(ConstructionTARGET) $(ConstructionOBJECTS)




# OPST + mining maximal patterns


maximalOPSTSOURCES = OPST_MP.cpp node.cpp OPST.cpp utils.cpp
maximalOPSTOBJECTS = $(maximalOPSTSOURCES:.cpp=.o)
maximalOPSTTARGET = OPSTMaximal




$(maximalOPSTTARGET): $(maximalOPSTOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_OPSTMaximal:
	rm -f $(maximalOPSTTARGET) $(maximalOPSTOBJECTS)






# OPST + mining closed patterns


closedOPSTSOURCES = OPST_CP.cpp node.cpp OPST.cpp utils.cpp
closedOPSTOBJECTS = $(closedOPSTSOURCES:.cpp=.o)
closedOPSTTARGET = OPSTClosed



$(closedOPSTTARGET): $(closedOPSTOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_OPSTClosed:
	rm -f $(closedOPSTTARGET) $(closedOPSTOBJECTS)











# baseline for mining maximal patterns

BAMPSOURCES = BA_MP.cpp baseline.cpp utils.cpp
BAMPOBJECTS = $(BAMPSOURCES:.cpp=.o)
BAMPTARGET = BAMP



$(BAMPTARGET): $(BAMPOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_BAMP:
	rm -f $(BAMPTARGET) $(BAMPOBJECTS)



# baseline for mining closed patterns


BACPSOURCES = BA_CP.cpp baseline.cpp utils.cpp
BACPOBJECTS = $(BACPSOURCES:.cpp=.o)
BACPTARGET = BACP



$(BACPTARGET): $(BACPOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean_BACP:
	rm -f $(BACPTARGET) $(BACPOBJECTS)


all: $(ConstructionTARGET) $(maximalOPSTTARGET) $(closedOPSTTARGET) $(BAMPTARGET) $(BACPTARGET)


clean: clean_OPSTConstruction clean_OPSTMaximal clean_OPSTClosed clean_BAMP clean_BACP


.PHONY: all clean clean_OPSTConstruction clean_OPSTMaximal clean_OPSTClosed clean_BAMP clean_BACP
