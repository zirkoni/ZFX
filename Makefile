CXX      = g++
CPPFLAGS = -IZFX -IZFX/include -I/usr/include/freetype2 -Itinyobjloader
CXXFLAGS = -g -O2 -MMD -std=c++17
LDFLAGS  = 
LIBS     = -lGL -lGLEW -lSDL2 -lfreetype

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
