#pragma once
#include <string>
#include <iostream>
#include <raylib.h>
#include <cassert>
#include <array>
#include "Settings.h"
#if D3
	#include "MarchingCubes.h"
#else
	#include "MarchingSquares.h"
#endif



class Game {
public:
	Game(int width, int height, int fps, const std::string& title);
	~Game() noexcept;
	Game& operator=(const Game& other) = delete;
	Game(const Game& other) = delete;
	[[nodiscard]] bool GameShouldClose() const;
	void Tick();

private:
	void Draw();
	void Update();
	void Init();

	Camera3D camera;
	size_t lines = 0;

	std::array<std::pair<Vector2, Vector2>, (size_t) TOTAL>* line_buffer;
	osn_context* ctx;
	int x = 0, y = 0, z = 0;

	Source src;
};
