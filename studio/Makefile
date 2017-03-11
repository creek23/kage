# Makefile for KageStudio
BIN = kage
CXX = g++
CXXFLAGS = -c -I./ `pkg-config gtkmm-3.0 --cflags | sed 's/ -I/ -isystem /g'` -std=gnu++11 -fmax-errors=5 -Wfatal-errors
CXXLINKS = `pkg-config gtkmm-3.0 --libs | sed 's/ -I/ -isystem /g'`
RES = windres
RESFILE = kage.res
RCFILE = kage.rc

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

$(RESFILE):
	$(RES) $(RCFILE) -O coff -o $(RESFILE)

kagestudio: $(OBJECTS) $(RESFILE)
	@echo creating $(BIN)
	$(CXX) $(OBJECTS) $(RESFILE) -o $(BIN) $(CXXLINKS)

clean:
	@echo deleting .o, $(RESFILE), and $(BIN)
	rm $(OBJECTS) $(RESFILE) $(BIN)
