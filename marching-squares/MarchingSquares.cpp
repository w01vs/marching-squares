#include "MarchingSquares.h"

[[nodiscard]] int x(const int idx) { return idx % WIDTH; }

[[nodiscard]] int y(const int idx) { return idx / WIDTH; }

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

std::array<float, (size_t)TOTAL>* gen_source()
{
	std::array<float, (size_t)TOTAL>* res = new std::array<float, (size_t)TOTAL>;
	for(int i = 0; i < TOTAL; ++i) { (*res)[i] = random(0, 1); }

	return res;
}

void draw_point(const int index, const Color c) { DrawRectangle(x(index) * CELL - 2, y(index) * CELL - 2, 4, 4, c); }

void draw_case(const int idx, float data[], float cases[], const Color color = GREEN, const bool interpolate = false)
{
	const float y_ = (float)y(idx) * (CELL * 1);
	const float x_ = (float)x(idx) * (CELL * 1);
	// data order:
	// top left
	// top right
	// bottom right
	// bottom left

	// Predefined edge points
	Vector2 top_edge;
	Vector2 left_edge;
	Vector2 right_edge;
	Vector2 bottom_edge;
	if(interpolate)
	{
		top_edge = { x_ + (float)CELL / 2 + data[0] * (float)CELL / 2 + data[1] * (float)CELL / 2, y_ };
		left_edge = { x_, y_ + (float)CELL / 2 + data[0] * (float)CELL / 2 + data[3] * (float)CELL / 2 };
		right_edge = { x_ + (float)CELL, y_ + (float)CELL / 2 + data[1] * (float)CELL / 2 + data[2] * (float)CELL / 2 };
		bottom_edge = {x_ + (float)CELL / 2 + data[2] * (float)CELL / 2 + data[3] * (float)CELL / 2, y_ + (float)CELL };
	}
	else
	{
		top_edge = { x_ + (float)CELL / 2, y_ };
		left_edge = { x_, y_ + (float)CELL / 2 };
		right_edge = { x_ + (float)CELL, y_ + (float)CELL / 2 };
		bottom_edge = { x_ + (float)CELL / 2, y_ + (float)CELL };
	}
	switch((unsigned char)(cases[0] + (cases[1] * 2) + (cases[2] * 4) + (cases[3] * 8)))
	{
	case 1:
	case 14:
		DrawLineV(top_edge, left_edge, color);
		break;
	case 2:
	case 13:
		DrawLineV(top_edge, right_edge, color);
		break;
	case 4:
	case 11:
		DrawLineV(right_edge, bottom_edge, color);
		break;
	case 5:
		DrawLineV(top_edge, left_edge, color);
		DrawLineV(right_edge, bottom_edge, color);
		break;
	case 7:
	case 8:
		DrawLineV(bottom_edge, left_edge, color);
		break;
	case 6:
	case 9:
		DrawLineV(top_edge, bottom_edge, color);
		break;
	case 10:
		DrawLineV(top_edge, right_edge, color);
		DrawLineV(bottom_edge, left_edge, color);
		break;
	case 3:
	case 12:
		DrawLineV(left_edge, right_edge, color);
		break;
	default:
		break;
	}
}

void march_squares(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	for(int i = 0; i < (int)arr->size() - WIDTH; ++i)
	{
		if(i > WIDTH && i % WIDTH == WIDTH - 1)
			continue;
		// For now use 0 or 1, interpolation later
		float vals[4] = {
			arr->at(i),
			arr->at(i + 1),
			arr->at(i + WIDTH + 1),
			arr->at(i + WIDTH)
		};

		for(int j = 0; j < 4; j++)
		{
			vals[j] = (vals[j] + 1.0f) / 2.0f;
			vals[j] = vals[j] < INTERPOLATE_THRESHOLD ? -vals[j] : vals[j];
		}
		float pure_vals[4] = {arr->at(i) < ACTIVE_THRESHOLD ? 0.0f : 1.0f, arr->at(i + 1) < ACTIVE_THRESHOLD ? 0.0f : 1.0f, arr->at(i + WIDTH + 1) < ACTIVE_THRESHOLD ? 0.0f : 1.0f, arr->at(i + WIDTH) < ACTIVE_THRESHOLD ? 0.0f : 1.0f};
		draw_case(i, vals, pure_vals, GREEN, true);
	}
}

void draw_points(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	for(int i = 0; i < (int)arr->size(); ++i)
	{
		const float cf = arr->at(i) * 255;
		const auto c = (unsigned char)(cf < 0 ? 0 : cf);
		const auto color = Color{ c, c, c, 255 }; // for interpolation stuff
		//const auto color = arr[i] < THRESHOLD ? BLACK : WHITE;
		if(c > 122) 
			draw_point(i, color);
	}
}

void draw_inside(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	for(int i = 0; i < (int)arr->size(); ++i)
	{
		Color color;
		if(arr->at(i) <= 0) color = BLACK;
		else
		{
			const auto c = (unsigned char)(arr->at(i) * 255);
			color = Color{c, c, c, 255};
		}
		DrawRectangle(x(i) * CELL, y(i) * CELL, CELL, CELL, color);
	}
}
 
void print_points(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	std::cout << "{\n";
	int w = WIDTH;
	for(int i = 0; i < (int)arr->size() - 1; ++i)
	{
		if(w == 0)
		{
			w = WIDTH;
			std::cout << "\n";
		}
		std::cout << arr->at(i) << ", ";
		w--;
	}

	std::cout << arr->at(arr->size() - 1) << " ";
	std::cout << "\n}" << std::endl;
}

void sample_noise(Source& src)
{
	osn_context* ctx;
	open_simplex_noise(1234, &ctx); // 1234 is the seed
	
	for(int i = 0; i < (int)src.arr->size(); i++)
	{
		const int _x = i % WIDTH;
		const int _y = i / WIDTH;

		src.xoff = (float)_x * src.inc;
		src.yoff = (float)_y * src.inc;
		(*src.arr)[i] = (float)open_simplex_noise3(ctx, src.xoff, src.yoff, src.zoff);
	}

	open_simplex_noise_free(ctx);
	src.zoff += src.z_inc;
}



