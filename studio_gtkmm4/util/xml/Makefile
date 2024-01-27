BIN = testBasicXml
CXX = g++
CXXFLAGS = -c -I./ -std=gnu++11 -fmax-errors=5 -Wfatal-errors -g
CXXLINKS = 

SOURCES = \
	xmltoken.cpp \
	xmltagproperty.cpp \
	xmltag.cpp \
	xml.cpp \
	testBasicXml.cpp

OBJECTS = $(SOURCES:.cpp=.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

all: kagestudio

kagestudio: $(OBJECTS)
	@echo creating $(BIN)
	$(CXX) $(OBJECTS) -o $(BIN) $(CXXLINKS)

clean:
	@echo deleting .o, and $(BIN)
	rm $(OBJECTS) $(BIN)
