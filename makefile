linux:
	g++ -std=c++17 main.cpp -o prog -lSDL2 -lSDL2_image -lSDL_mixer -ldl
	./prog
windows:
