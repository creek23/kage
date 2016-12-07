# Makefile for KageStudio
BIN = kage_gtkmm3
CXX = g++
CXXFLAGS = -c -I./ `pkg-config gtkmm-3.0 --cflags | sed 's/ -I/ -isystem /g'` -std=gnu++11 -fmax-errors=5 -Wfatal-errors
CXXLINKS = `pkg-config gtkmm-3.0 --libs | sed 's/ -I/ -isystem /g'`
RES = windres
RESFILE = kage.res
RCFILE = kage.rc

SOURCES = \
	./kage/data/point_gtkmm3.cpp \
	./kage/data/color_gtkmm3.cpp \
	./kage/data/strokecolor_gtkmm3.cpp \
	./kage/data/vectordata_gtkmm3.cpp \
	./kage/timeline/frame_gtkmm3.cpp \
	./kage/timeline/framemanager_gtkmm3.cpp \
	./kage/timeline/framesmanager_gtkmm3.cpp \
	./kage/timeline/layermanager_gtkmm3.cpp \
	./kage/stage_gtkmm3.cpp \
	./kage/vectordatamanager_gtkmm3.cpp \
	kage_gtkmm3.cpp \
	main_gtkmm3.cpp

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
