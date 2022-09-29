CFLAGS = -O3 -Wall -Wno-pointer-sign -fno-strict-aliasing 
CC = gcc

bin  =	game
objs = testing

all: $(bin)

clean:
	rm -f core *.o $(bin)

game :
	$(CC) $(CFLAGS) game.c -o $@ -L/opt/homebrew/lib -I/opt/homebrew/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer window_management.c input_management.c graphics_management.c asset_management.c audio_management.c game_logic.c
