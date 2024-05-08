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
	void Draw();
	void Update();
	void Init();

	bool D3 = false;
	Camera3D camera;
	size_t lines = 0;

	std::array<std::pair<Vector2, Vector2>, (size_t) TOTAL>* line_buffer;

	Source src;
};
