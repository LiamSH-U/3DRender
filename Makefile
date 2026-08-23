CXX = g++

CXXFLAGS = -Wall -Wextra
CPPFLAGS = -Iinclude

PROGRAM = saveimg

SRCS = src/main.cpp src/Textures.cpp src/FrameBuffer.cpp src/Map.cpp src/rendering_utils.cpp src/rendering.cpp src/user_specs.cpp

DBG_DIR = build/debug
DBG_OBJ_DIR = $(DBG_DIR)/objs
DBG_DEP_DIR = $(DBG_DIR)/deps

REL_DIR = build/release
REL_OBJ_DIR = $(REL_DIR)/objs
REL_DEP_DIR = $(REL_DIR)/deps

REL_OBJS = $(SRCS:src/%.cpp=$(REL_OBJ_DIR)/%.o)
DBG_OBJS = $(SRCS:src/%.cpp=$(DBG_OBJ_DIR)/%_dbg.o)

REL_DEPS = $(REL_OBJS:.o=.d)
DBG_DEPS = $(DBG_OBJS:.o=.d)

.PHONY: all debug release clean

all: release debug


release: $(REL_DIR)/$(PROGRAM)

$(REL_DIR)/$(PROGRAM): $(REL_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -O3 $^ -o $@

$(REL_OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D) $(REL_DEP_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -O3 -MMD -MP -MF $(REL_DEP_DIR)/$*.d -c $< -o $@


debug: $(DBG_DIR)/$(PROGRAM)_dbg

$(DBG_DIR)/$(PROGRAM)_dbg: $(DBG_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -g3 $^ -o $@

$(DBG_OBJ_DIR)/%_dbg.o: src/%.cpp
	@mkdir -p $(@D) $(DBG_DEP_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g3 -MMD -MP -MF $(DBG_DEP_DIR)/$*_dbg.d -c $< -o $@


-include $(REL_DEPS)
-include $(DBG_DEPS)

topdown.gif:
	convert -delay 5 -loop 0 topdown*.ppm topdown.gif
	rm -f topdown*.ppm

playerview.gif:
	convert -delay 5 -loop 0 playerview*.ppm playerview.gif
	rm -f playerview*.ppm

gifs:
	convert -delay 5 -loop 0 playerview*.ppm playerview.gif & convert -delay 5 -loop 0 topdown*.ppm topdown.gif & wait
	rm -f *.ppm

run:
	./build/release/saveimg

animation: run gifs

clean:
	rm -rf build *.ppm *.gif