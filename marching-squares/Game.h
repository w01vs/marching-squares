#pragma once
#include <string>
#include <iostream>
#include <raylib.h>
#include <cassert>
#include <array>
#include "MarchingSquares.h"


class Game {
public:
	Game(int width, int height, int fps, const std::string& title, bool D3 = false);
	~Game() noexcept;
	Game& operator=(const Game& other) = delete;
	Game(const Game& other) = delete;
	[[nodiscard]] bool GameShouldClose() const;
	void Tick();

private:
	void Draw() const;
	void Update();
	void Init();

	bool D3 = false;
	Camera3D camera;

	Source src;
};
