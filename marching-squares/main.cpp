#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Settings.h"

int main() {
	Game game{ SCREEN_WIDTH, SCREEN_HEIGHT, FPS, "Game Window" };


	while (!game.GameShouldClose()) {
		game.Tick();
	}

	return 0;
}