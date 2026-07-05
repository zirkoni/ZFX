.PHONY: all clean

# Example:
# make CMAKE_ARGS="-DUSE_STB_IMAGE=ON -DCMAKE_BUILD_TYPE=Release"
CMAKE_ARGS ?=

all:
	mkdir -p build
	cd build && cmake $(CMAKE_ARGS) .. && make -j$(nproc)

clean:
	rm -rf build
