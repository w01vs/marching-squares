#pragma once
#include <iostream>
#include <raylib.h>
#include <array>
#include "Settings.h"
#include <random>
#include "OpenSimplex.h"

#define INTERPOLATE_THRESHOLD 0.5f
#define ACTIVE_THRESHOLD 0

enum
{
	CELL = 10,
	WIDTH = SCREEN_WIDTH / CELL + 1,
	HEIGHT = SCREEN_HEIGHT / CELL + 1,
	TOTAL = WIDTH * HEIGHT
};

struct Source
{
	std::array<float, (size_t)TOTAL> arr;
	float xoff = 0;
	float yoff = 0;
	float inc = 0.085f;
	float z_inc = 0.00035f;
	float zoff = 0;
};

void draw_inside(const Source& src);
void march_squares(const Source& src);
void sample_noise(Source& src);
void draw_points(const Source& src);
void print_points(const Source& src);
float random(float, float);
std::array<float, (size_t)TOTAL> gen_source();
[[nodiscard]] int x(int);
[[nodiscard]] int y(int);