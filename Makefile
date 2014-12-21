CXX = g++

# Colourized output. Call with: $(call print, "z0mg t3h culurz!")
define print
      @tput setaf 6
      @echo $1
      @tput sgr0
endef

# Project information.
SOURCES = main.cpp Gif.cpp GifException.cpp Image.cpp Animation.cpp Color.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = Gif2UnicornHat

# 3rd Party Libraries
WS2812PATH = ./UnicornHat/python/ws2812/lib/
LDFLAGS += -lpthread -lgif -L$(WS2812PATH) -lws2812-RPi

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wredundant-decls -Wunreachable-code
STRICTNESS = -pedantic
INCLUDES += -isystem $(WS2812PATH)
CXXFLAGS = -std=c++11 -D_POSIX_SOURCE $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# Targets
all: $(SOURCES) $(EXECUTABLE)

dependencies:
	make -C $(WS2812PATH)

.cpp.o:
	$(call print, "Compiling $< into $@")
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(call print, "Linking $(OBJECTS) into $@")
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	$(call print, "Cleaning...")
	rm -f $(OBJECTS) $(EXECUTABLE)
	$(call print, "Done cleaning.")
