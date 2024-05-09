#include "MarchingSquares.h"
#include <thread>
#include <mutex>

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
	for(int i = 0; i < TOTAL; ++i) { (*res)[i] = random(-1, 1); }

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

std::optional<std::vector<std::pair<Vector2, Vector2>>> get_lines(const int idx, float data[], float cases[])
{
	std::vector<std::pair<Vector2, Vector2>> ret;
	const float y_ = (float)y(idx) * (CELL * 1);
	const float x_ = (float)x(idx) * (CELL * 1);
	// data order:
	// top left
	// top right
	// bottom right
	// bottom left

	// Predefined edge points
	Vector2 top_edge = {x_ + (float)CELL / 2, y_};
	Vector2 left_edge = {x_, y_ + (float)CELL / 2};
	Vector2 right_edge = {x_ + (float)CELL, y_ + (float)CELL / 2};
	Vector2 bottom_edge = {x_ + (float)CELL / 2, y_ + (float)CELL};

	switch((unsigned char)(cases[0] + (cases[1] * 2) + (cases[2] * 4) + (cases[3] * 8)))
	{
		case 1:
		case 14:
			ret.emplace_back(top_edge, left_edge);
			break;
		case 2:
		case 13:
			ret.emplace_back(top_edge, right_edge);
			break;
		case 4:
		case 11:
			ret.emplace_back(right_edge, bottom_edge);
			break;
		case 5:
			ret.emplace_back(top_edge, left_edge);
			ret.emplace_back(right_edge, bottom_edge);
			break;
		case 7:
		case 8:
			ret.emplace_back(bottom_edge, left_edge);
			break;
		case 6:
		case 9:
			ret.emplace_back(top_edge, bottom_edge);
			break;
		case 10:
			ret.emplace_back(top_edge, right_edge);
			ret.emplace_back(bottom_edge, left_edge);
			break;
		case 3:
		case 12:
			ret.emplace_back(left_edge, right_edge);
			break;
		default:
			return {};
	}
	return ret;
}

void threaded_squares(const int begin, const int end, const Source& src, std::vector<std::vector<std::optional<std::vector<std::pair<Vector2, Vector2>>>>>* res, std::mutex* lock)
{
	std::vector<std::optional<std::vector<std::pair<Vector2, Vector2>>>> internal_res;
	for(int i = begin; i < end; ++i)
	{
		internal_res.emplace_back(march_square(src, i));
	}
	lock->lock();
	res->emplace_back(internal_res);
	lock->unlock();
}

void march_squares(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	
	// disabled since it dont work, maybe ill fix it later
	if(TOTAL >= 1000)
	{
		std::vector<std::vector<std::optional<std::vector<std::pair<Vector2, Vector2>>>>> res;
		std::mutex lock;
		std::vector<std::thread> threads;
		const auto max_threads = std::thread::hardware_concurrency();
		const int thread_count = THREADS > max_threads ? max_threads : THREADS;
		threads.reserve(thread_count);

		const int thread_range = TOTAL / thread_count;

		for(int i = 0; i < thread_count; i++)
		{
			const int begin = i * thread_range;
			const int end = (i + 1) * thread_range;
			threads.emplace_back(threaded_squares, begin, end, std::ref(src), &res, &lock);
		}

		for(int i = 0; i < thread_count; i++)
		{
			threads[i].join();
		}

		for(int i = 0; i < res.size(); i++)
		{
			for(int j = 0; j < res[i].size(); j++)
			{
				if(res[i][j].has_value())
				{
					const auto line = res[i][j].value();
					for(int l = 0; l < line.size(); l++)
					{
						DrawLineV(line[l].first, line[l].second, GREEN);
					}
				}
			}
		}
	}
	else
	{
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
			draw_case(i, vals, pure_vals, GREEN, false);
		}
	}
}

std::optional<std::vector<std::pair<Vector2, Vector2>>> march_square(const Source& src, const int i)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	// For now use 0 or 1, interpolation later
	if((i > WIDTH && i % WIDTH == WIDTH - 1) || i >= TOTAL - WIDTH)
		return {};
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
	return get_lines(i, vals, pure_vals);
}

void draw_points(const Source& src)
{
	const std::array<float, (size_t)TOTAL>* arr = src.arr;
	for(int i = 0; i < (int)arr->size(); ++i)
	{
		const float cf = arr->at(i) * 255;
		const auto c = (unsigned char)(cf < 0 ? 0 : cf);
		const auto color = Color{ c, c, c, 255 }; // for interpolation stuff
		//const auto color = (*arr)[i] < ACTIVE_THRESHOLD ? BLACK : WHITE;
		if(c > 0) 
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


void threaded_noise(const int start, const int end, const Source* src, const osn_context* ctx)
{
	for(int i = start; i < end; i++)
	{
		const int _x = i % WIDTH;
		const int _y = i / WIDTH;

		const auto value = (float)open_simplex_noise3(ctx, (float)_x * src->inc, (float)_y * src->inc, src->zoff);
		
		(*src->arr)[i] = value;
	}
}

void sample_noise(Source& src, osn_context* ctx)
{
	open_simplex_noise(1234, &ctx); // 1234 is the seed

	if(TOTAL >= 1000)
	{
		std::vector<std::thread> threads;
		const auto max_threads = std::thread::hardware_concurrency();
		const int thread_count = THREADS > max_threads ? max_threads : THREADS;
		threads.reserve(thread_count);

		const int thread_range = TOTAL / thread_count;

		for(int i = 0; i < thread_count; i++)
		{
			const int begin = i * thread_range;
			const int end = (i + 1) * thread_range;
			threads.emplace_back(threaded_noise, begin, end, &src, ctx);
		}

		for(int i = 0; i < thread_count; i++)
		{
			threads[i].join();
		}
	}
	else
	{
		for(int i = 0; i < (int)src.arr->size(); i++)
		{
			const int _x = i % WIDTH;
			const int _y = i / WIDTH;

			(*src.arr)[i] = (float)open_simplex_noise3(ctx, (float)_x * src.inc, (float)_y * src.inc, src.zoff);
		}
	}
	open_simplex_noise_free(ctx);
	src.zoff += src.z_inc;
}



