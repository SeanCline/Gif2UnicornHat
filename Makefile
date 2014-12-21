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
PCH = pch.h

# Build Flags
OPTIMIZATION_LEVEL = -O0 -g -ggdb
WARNINGS = -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wredundant-decls -Wunreachable-code -Winvalid-pch
STRICTNESS = -pedantic
INCLUDES += -isystem UnicornHat/python/ws2812/lib/
CXXFLAGS = -std=c++11 -D_POSIX_SOURCE $(OPTIMIZATION_LEVEL) $(WARNINGS) $(STRICTNESS)

# 3rd Party Libraries
LDFLAGS += -lcurl -lcurlpp -lwiringPi -lpthread -lboost_program_options -lgif -L./UnicornHat/python/ws2812/lib/ -lws2812-RPi

# Targets
all: $(SOURCES) $(EXECUTABLE)

dependencies:
	make -C UnicornHat/python/ws2812/lib

#$(PCH).gch: $(PCH)
#	$(call print, "Building precompiled header $< into $@")
#	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<
	
#$(OBJECTS):
.cpp.o:
	$(call print, "Compiling $< into $@")
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(call print, "Linking $(OBJECTS) into $@")
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	$(call print, "Cleaning...")
	rm -f $(OBJECTS) $(EXECUTABLE) $(PCH).gch
	$(call print, "Done cleaning.")
