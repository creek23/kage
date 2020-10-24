# Makefile for KageStudio
BIN = kagestudio
CXX = g++
CXXFLAGS = -c -I./ `pkg-config gtkmm-3.0 gdkmm-3.0 glibmm-2.4 --cflags | sed 's/ -I/ -isystem /g'` -std=gnu++11 -fmax-errors=5 -Wfatal-errors
CXXLINKS = `pkg-config gtkmm-3.0 gdkmm-3.0 glibmm-2.4 --libs | sed 's/ -I/ -isystem /g'`

SOURCES = \
	./kage/data/anchor.cpp \
	./kage/data/point.cpp \
	./kage/data/color.cpp \
	./kage/data/strokecolor.cpp \
	./kage/data/vectordata.cpp \
	./kage/timeline/frame.cpp \
	./kage/timeline/frame_padding.cpp \
	./kage/timeline/frameset.cpp \
	./kage/timeline/framesmanager.cpp \
	./kage/timeline/layer.cpp \
	./kage/timeline/layermanager.cpp \
	./kage/unre/kagedo.cpp \
	./kage/unre/unre.cpp \
	./kage/stage.cpp \
	./kage/stage_node.cpp \
	./kage/stage_oval.cpp \
	./kage/stage_poly.cpp \
	./kage/stage_rect.cpp \
	./kage/stage_pencil.cpp \
	./kage/stage_shape.cpp \
	./kage/vectordatamanager.cpp \
	./util/xml/xmltoken.cpp \
	./util/xml/xmltagproperty.cpp \
	./util/xml/xmltag.cpp \
	./util/xml/xml.cpp \
	./util/string/stringhelper.cpp \
	./util/cairo/cairo_kage.cpp \
	kage.cpp \
	resources.cpp \
	about.cpp \
	layer_rename.cpp \
	main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

all: kagestudio

resource:
	glib-compile-resources --generate-source --target=resources.cpp image.gresource.xml

kagestudio: $(OBJECTS)
	@echo creating $(BIN)
	$(CXX) $(OBJECTS) -o $(BIN) $(CXXLINKS)

clean:
	@echo deleting .o, and $(BIN)
	rm $(OBJECTS) $(BIN)
