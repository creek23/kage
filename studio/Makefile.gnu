# Makefile for KageStudio
BIN = kagestudio
CXX = g++
CXXFLAGS = -c -I./ `pkg-config gtkmm-3.0 gdkmm-3.0 glibmm-2.4 --cflags | sed 's/ -I/ -isystem /g'` -std=gnu++17 -fmax-errors=5 -Wfatal-errors
CXXLINKS = `pkg-config gtkmm-3.0 gdkmm-3.0 glibmm-2.4 --libs | sed 's/ -I/ -isystem /g'`

SOURCES = \
	./kage/data/anchor.cpp \
	./kage/data/point.cpp \
	./kage/data/color.cpp \
	./kage/data/strokecolor.cpp \
	./kage/data/vectordata.cpp \
	./kage/timeline/layers.cpp \
	./kage/timeline/scenes.cpp \
	./kage/timeline/timeline.cpp \
	./kage/unre/kagedo.cpp \
	./kage/unre/unre.cpp \
	./kage/stage/stage.cpp \
	./kage/stage/node.cpp \
	./kage/stage/oval.cpp \
	./kage/stage/poly.cpp \
	./kage/stage/rect.cpp \
	./kage/stage/pencil.cpp \
	./kage/stage/shape.cpp \
	./kage/library/asset.cpp \
	./kage/library/assetmanager.cpp \
	./kage/library/library.cpp \
	./kage/properties/asset.cpp \
	./kage/properties/fillstroke.cpp \
	./kage/properties/frametween.cpp \
	./kage/properties/locationsize.cpp \
	./kage/properties/nodexy.cpp \
	./kage/properties/stage.cpp \
	./kage/vectordatamanager.cpp \
	./kage/data/frame.cpp \
	./kage/data/layer.cpp \
	./kage/data/scene.cpp \
	./kage/document.cpp \
	./util/xml/xmltoken.cpp \
	./util/xml/xmltagproperty.cpp \
	./util/xml/xmltag.cpp \
	./util/xml/xml.cpp \
	./util/string/stringhelper.cpp \
	./util/cairo/cairo_kage.cpp \
	kage.cpp \
	resources.cpp \
	about.cpp \
	label_rename.cpp \
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
