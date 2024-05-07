#pragma once
#include <string>
#include <iostream>
#include <raylib.h>
#include <cassert>
#include <array>
#include "MarchingSquares.h"


class Game {
public:
	Game(int width, int height, int fps, std::string title);
	~Game() noexcept;
	Game& operator=(const Game& other) = delete;
	Game(const Game& other) = delete;
	bool GameShouldClose() const;
	void Tick();

private:
	void Draw();
	void Update();
	void Init();

	Source src;
};
