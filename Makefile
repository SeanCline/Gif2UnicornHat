CXX = g++
AR = ar

# Colourized output. Call with: $(call print, "z0mg t3h culurz!")
define print
      @tput setaf 6
      @echo $1
      @tput sgr0
endef

# Project information.
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
LIBRARYOBJECTS = $(filter-out ProgramOptions.o, $(filter-out main.o, $(OBJECTS)))
EXECUTABLE = Gif2UnicornHat
LIBRARY = libGif2UnicornHat.a

# 3rd Party Libraries
WS2812PATH = ./UnicornHat/python/ws2812/lib/
LDFLAGS += -lpthread -lgif -L$(WS2812PATH) -lws2812-RPi

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wredundant-decls -Wunreachable-code
STRICTNESS = -pedantic
INCLUDES += -isystem $(WS2812PATH)
CXXFLAGS = -std=c++0x -D_POSIX_SOURCE -fPIC $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# Targets
all: $(SOURCES) $(EXECUTABLE) $(LIBRARY)

dependencies:
	make -C $(WS2812PATH)

.cpp.o:
	$(call print, "Compiling $< into $@")
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(call print, "Linking $(OBJECTS) into executable $@")
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(LIBRARY): $(LIBRARYOBJECTS)
	$(call print, "Archiving $(LIBRARYOBJECTS) into library $@")
	@$(AR) rcs $@ $(LIBRARYOBJECTS)
	
clean:
	$(call print, "Cleaning...")
	@rm -f $(OBJECTS) $(EXECUTABLE) $(LIBRARY)
	$(call print, "Done cleaning.")
