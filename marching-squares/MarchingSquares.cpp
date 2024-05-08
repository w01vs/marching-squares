#include "MarchingSquares.h"

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

void draw_case(const int idx, float data[])
{
	const int y_ = y(idx) * CELL;
	const int x_ = x(idx) * CELL;
	// data order:
	// top left
	// top right
	// bottom right
	// bottom left
	switch((unsigned char)(data[0] + (data[1] * 2) + (data[2] * 4) + (data[3] * 8)))
	{
		case 1:
		case 14:
		/*
		 move the line according to point values.
		 in case 14
		 *--/-o
		 | /  |
		 |/   |
		 |    |
		 o----o

		 the top left corner is lower than  0.5f
		 move the top edge intersection to the right with topleft/ topleft+topright / 2 * CELL
		 move the left edge intersection to the top with topleft/ topleft+bottomleft / 2 * CELL
		*/
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
		float vals[4] = {arr[i] < 0.5 ? 0.0f : 1.0f, arr[i + 1] < 0.5 ? 0.0f : 1.0f, arr[i + WIDTH + 1] < 0.5 ? 0.0f : 1.0f, arr[i + WIDTH] < 0.5 ? 0.0f : 1.0f};
		float pure_vals[4] = { arr[i] , arr[i + 1], arr[i + WIDTH + 1], arr[i + WIDTH] };
		draw_case(i, vals);
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



