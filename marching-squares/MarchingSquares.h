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
	CELL = 8,
	WIDTH = SCREEN_WIDTH / CELL + 1,
	HEIGHT = SCREEN_HEIGHT / CELL + 1,
	TOTAL = WIDTH * HEIGHT
};

struct Source
{
	std::array<float, (size_t)TOTAL>* arr;
	float xoff = 0;
	float yoff = 0;
	float inc = 0.085f;
	float z_inc = 0.01f;
	float zoff = 0;

	Source() : arr(nullptr) {}

	explicit Source(std::array<float, (size_t)TOTAL>* arr) : arr(arr) {}

	Source(const Source& other) = delete;
	Source& operator=(const Source& other) = delete;

	Source(Source&& other) = delete;
	Source& operator=(Source&& other) noexcept
	{
		if(this == &other) return *this;

		delete arr;
		arr = other.arr;
		other.arr = nullptr;
		xoff = other.xoff;
		yoff = other.yoff;
		inc = other.inc;
		z_inc = other.z_inc;
		zoff = other.zoff;

		return *this;
	}

	~Source()
	{
		delete arr;
	}
};

void draw_inside(const Source& src);
void march_squares(const Source& src);
void sample_noise(Source& src);
void draw_points(const Source& src);
void print_points(const Source& src);
float random(float, float);
std::array<float, (size_t)TOTAL>* gen_source();
[[nodiscard]] int x(int);
[[nodiscard]] int y(int);