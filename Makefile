# Makefile by Snoolie K / 0xilis (me!). Apologies if it is not the best.

output: ./build/init.o ./build/seajson.o ./build/resource_management.o ./build/emu.o
	@if [ -d "./build/out" ]; \
	then \
		clang ./build/init.o ./build/seajson.o ./build/resource_management.o ./build/emu.o -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -I/usr/local/include/SDL2 -D_THREAD_SAFE -o ./build/out/Peppermint; \
		mv ./build/out/Peppermint ./Chippy; \
	else \
		echo "Oh my god, please create ./build/out directory before running make, you heartless bastard!"; \
		exit 1; \
	fi

./build/init.o: ./src/init.c
	@if [ -d "./build" ]; \
	then \
		clang -c ./src/init.c -Os -o ./build/init.o; \
	else \
		echo "Oh my god, please create ./build directory before running make, you heartless bastard!"; \
		exit 1; \
	fi


./build/seajson.o: ./src/seajson.c
	@if [ -d "./build" ]; \
	then \
		clang -c ./src/seajson.c -Os -o ./build/seajson.o; \
	else \
		echo "Oh my god, please create ./build directory before running make, you heartless bastard!"; \
		exit 1; \
	fi

./build/resource_management.o: ./src/resource_management.c
	@if [ -d "./build" ]; \
	then \
		clang -c ./src/resource_management.c -Os -o ./build/resource_management.o; \
	else \
		echo "Oh my god, please create ./build directory before running make, you heartless bastard!"; \
		exit 1; \
	fi

./build/emu.o: ./src/emu.c
	@if [ -d "./build" ]; \
	then \
		clang -c ./src/emu.c -Os -o ./build/emu.o; \
	else \
		echo "Oh my god, please create ./build directory before running make, you heartless bastard!"; \
		exit 1; \
	fi
