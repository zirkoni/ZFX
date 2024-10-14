CXX      = g++
CXXFLAGS = -g -O2 -MMD -std=c++17
LDFLAGS  = 

CPPFLAGS += -IZFX
CPPFLAGS += -IZFX/src
CPPFLAGS += -Itinyobjloader

ifeq ($(OS),Windows_NT)
    $(info Compiling for Windows)

    ifneq ($(MSYSTEM),UCRT64)
        $(error Not mingw-w64-ucrt environment)
        exit
    endif

    CPPFLAGS += -I/ucrt64/include/freetype2

    LIBS += -lOpenGL32
    LIBS += -lglew32
    LIBS += -lmingw32
    LIBS += -lSDL2main
    LIBS += -lSDL2
    LIBS += -lfreetype
else
    $(info Compiling for Linux)

    CPPFLAGS += -I/usr/include/freetype2

    LIBS += -lGL
    LIBS += -lGLEW
    LIBS += -lSDL2
    LIBS += -lfreetype
endif

cppsrc = $(wildcard ZFX/src/*.cpp) $(wildcard Demo/*.cpp)
ccsrc  = tinyobjloader/tiny_obj_loader.cc
obj    = $(cppsrc:.cpp=.o) $(ccsrc:.cc=.o)
dep    = $(obj:.o=.d)

zfx_demo: $(obj)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(obj) zfx_demo
	rm -f $(dep)

-include $(dep)
