# Makefile for KageStudio
BIN = kagebin
CXX = g++
CXXFLAGS = -c -I./ `pkg-config gtkmm-3.0 gdkmm-3.0 cairo --cflags | sed 's/ -I/ -isystem /g'` -std=gnu++11 -fmax-errors=5 -Wfatal-errors
CXXLINKS = `pkg-config gtkmm-3.0 gdkmm-3.0 cairo --libs | sed 's/ -I/ -isystem /g'`

SOURCES = \
	./kage/data/anchor.cpp \
	./kage/data/point.cpp \
	./kage/data/color.cpp \
	./kage/data/strokecolor.cpp \
	./kage/data/vectordata.cpp \
	./kage/timeline/frame.cpp \
	./kage/timeline/framemanager.cpp \
	./kage/timeline/framesmanager.cpp \
	./kage/timeline/layermanager.cpp \
	./kage/stage.cpp \
	./kage/vectordatamanager.cpp \
	kage.cpp \
	main.cpp

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
