#pragma once
#include <iostream>
#include <raylib.h>
#include <array>
#include "Settings.h"
#include <random>

enum
{
	CELL = 20,
	WIDTH = SCREEN_WIDTH / CELL + 1,
	HEIGHT = SCREEN_HEIGHT / CELL + 1,
	TOTAL = WIDTH * HEIGHT
};

struct Source
{
	std::array<float, (size_t)TOTAL> arr;
};

void march_squares(const Source& src);
void draw_points(const Source& src);
float random(float, float);
std::array<float, (size_t)TOTAL> gen_source();
[[nodiscard]] int x(int);
[[nodiscard]] int y(int);