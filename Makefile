.PHONY: all clean

all:
	mkdir -p build
	cd build && cmake .. && make -j$(nproc)

clean:
	rm -rf build

