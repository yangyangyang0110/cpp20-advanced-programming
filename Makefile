all:
	cmake --build build --target all -j
configure:
	cmake -S . -B build