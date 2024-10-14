PROG_NAME = zfx_demo

CXX      = g++
CXXFLAGS = -g -O2 -MMD -std=c++17
LDFLAGS  = 

SRC_DIRS += Demo
SRC_DIRS += ZFX/src
SRC_DIRS += tinyobjloader

INC_DIRS += Demo
INC_DIRS += ZFX
INC_DIRS += ZFX/src
INC_DIRS += tinyobjloader

ifeq ($(OS),Windows_NT)
    $(info Compiling for Windows)

    ifneq ($(MSYSTEM),UCRT64)
        $(error Not mingw-w64-ucrt environment)
        exit
    endif

    INC_DIRS += /ucrt64/include/freetype2

    LIBS += OpenGL32
    LIBS += glew32
    LIBS += mingw32
    LIBS += SDL2main
    LIBS += SDL2
    LIBS += freetype
else
    $(info Compiling for Linux)

    INC_DIRS += /usr/include/freetype2

    LIBS += GL
    LIBS += GLEW
    LIBS += SDL2
    LIBS += freetype
endif

INC_FLAGS = $(addprefix -I,$(INC_DIRS))
LIB_FLAGS = $(addprefix -l,$(LIBS))

BUILD_DIR = build

SRCS_TMP = $(shell find $(SRC_DIRS) -name '*.cpp' -or -name 'tiny_obj_loader.cc')
SRCS = $(filter-out tinyobjloader/python/tiny_obj_loader.cc,$(SRCS_TMP))

OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

CPPFLAGS = $(INC_FLAGS) -MMD -MP
LDFLAGS += $(LIB_FLAGS)

$(BUILD_DIR)/$(PROG_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cc.o: %.cc
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
