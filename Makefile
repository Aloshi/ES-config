CXX=g++
CXXFLAGS=-Wall -g -O2

ANGELSCRIPT_ADD_ONS=angelscript_add_ons
PUGIXML=src/PugiXML
CPPFLAGS=-I/usr/include/SDL -I/usr/include -I/usr/local/include -I$(ANGELSCRIPT_ADD_ONS)
LIBS=-L/usr/local/lib -lSDL -lSDL_ttf -lSDL_image -langelscript

SOURCES=$(wildcard src/*.cpp) $(wildcard $(ANGELSCRIPT_ADD_ONS)/*/*.cpp) $(wildcard $(PUGIXML)/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=es-config

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(CPPFLAGS) $(OBJECTS) $(LIBS)

clean:
	rm -f src/*.o $(EXECUTABLE)
