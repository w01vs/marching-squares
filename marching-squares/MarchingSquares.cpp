#include "MarchingSquares.h"

struct Index
{
	int idx;
	explicit Index(const int index)
	{
		idx = index;
	}

	explicit Index(const int x, const int y)
	{
		idx = y * WIDTH + x;
	}

	Index operator+(const Index& other) const
	{
		return Index(idx + other.idx);
	}

	void operator++()
	{
		idx += 1;
	}

	void operator++(int)
	{
		idx += 1;
	}

	
};

[[nodiscard]] int x(const int idx)
{
	return idx % WIDTH;
}

[[nodiscard]] int y(const int idx)
{
	return idx / WIDTH;
}

// Range is inclusive on both ends
float random(const float min, const float max)
{
	if(min == max)
		return min;
	std::random_device rd;
	std::mt19937 eng(rd());

	std::uniform_real_distribution<> distr(min, max);
	return (float)distr(eng);
}

std::array<float, (size_t)TOTAL> gen_source()
{
	std::array<float, (size_t)TOTAL> res = {};
	for(int i = 0; i < TOTAL; ++i)
	{
		res[i] = random(0, 1);
	}

	return res;
}

void draw_point(const int index, const Color c)
{
	DrawRectangle(x(index) * CELL - 3, y(index) * CELL - 3, 6, 6, c);
}

void draw_case(const int idx, const unsigned char type)
{
	const int y_ = y(idx) * CELL;
	const int x_ = x(idx) * CELL;
	switch(type)
	{
	case 1:
	case 14:
		DrawLine(x_ + CELL / 2, y_, x_, y_ + CELL / 2, BLACK);
		break;
	case 2:
	case 13:
		DrawLine(x_ + CELL / 2, y_, x_ + CELL, y_ + CELL / 2, BLACK);
		break;
	case 4:
	case 11:
		DrawLine(x_ + CELL, y_ + CELL / 2, x_ + CELL / 2, y_ + CELL, BLACK);
		break;
	case 5:
		DrawLine(x_ + CELL / 2, y_, x_, y_ + CELL / 2, BLACK);
		DrawLine(x_ + CELL, y_ + CELL / 2, x_ + CELL / 2, y_ + CELL, BLACK);
		break;
	case 7:
	case 8:
		DrawLine(x_, y_ + CELL / 2, x_ + CELL / 2, y_ + CELL, BLACK);
		break;
	case 6:
	case 9:
		DrawLine(x_ + CELL / 2, y_, x_ + CELL / 2, y_ + CELL, BLACK);
		break;
	case 10:
		DrawLine(x_, y_ + CELL / 2, x_ + CELL / 2, y_ + CELL, BLACK);
		DrawLine(x_ + CELL / 2, y_, x_ + CELL, y_ + CELL / 2, BLACK);
		break;
	case 3:
	case 12:
		DrawLine(x_, y_ + CELL / 2, x_ + CELL, y_ + CELL / 2, BLACK);
		break;
	default:
		break;
	}
}

void march_squares(const Source& src)
{
	const std::array<float, (size_t)TOTAL> arr = src.arr;
	for(int i = 0; i < (int)arr.size() - WIDTH; ++i)
	{
		if(i > WIDTH && i % WIDTH == WIDTH - 1) continue;
		// For now use 0 or 1, interpolation later
		const int vals[4] = {arr[i] < 0.5 ? 0 : 1, arr[i + 1] < 0.5 ? 0 : 1, arr[i + WIDTH + 1] < 0.5 ? 0 : 1, arr[i + WIDTH] < 0.5 ? 0 : 1};
		draw_case(i, (unsigned char)(vals[0] + (vals[1] * 2) + (vals[2] * 4) + (vals[3] * 8)));
	}
}

void draw_points(const Source& src)
{
	const std::array<float, (size_t)TOTAL> arr = src.arr;
	for(int i = 0; i < (int)arr.size(); ++i)
	{
		if(i > WIDTH && i % WIDTH == WIDTH) continue;
		const auto c = (unsigned char)((arr[i] + 10) / 20 * 255);
		//const auto color = Color{ c, c, c, 255 }; // for interpolation stuff
		const auto color = arr[i] < 0.5 ? BLACK : WHITE;
		draw_point(i, color);
	}
}



