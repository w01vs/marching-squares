#include "Game.h"

Game::Game(const int width, const int height, int fps, const std::string& title)
{
	assert(!GetWindowHandle());
	InitWindow(width, height, title.c_str());
	Init();
	line_buffer = new std::array<std::pair<Vector2, Vector2>, TOTAL>;
}

Game::~Game() noexcept
{
	assert(GetWindowHandle());
	CloseWindow();
}

bool Game::GameShouldClose() const
{
	return WindowShouldClose();
}

void Game::Init()
{
	src = Source{gen_source()};
	if constexpr(VISUALISE)
	{
		sample_noise(src, ctx);
		src.woff = random(0, 25);
		src.w_inc = GetRandomValue(0, 1) == 0 ? -random(1, 5) : random(1, 5);
	}
}

void Game::Tick()
{
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

void Game::Draw()
{
	ClearBackground(BLACK);
	if constexpr(VISUALISE)
	{
		// Draw line by line marching squares
		if(lines != TOTAL - 1)
		{
			if(const auto new_lines = march_square(src, (int)lines); new_lines.has_value())
			{
				WaitTime(0.001);
				for(size_t i = 0; i < new_lines.value().size(); i++)
				{
					(*line_buffer)[lines] = new_lines.value()[i];
					lines++;
				}
			}
			else
				lines++;
		}
		else
		{
			src.w_inc = GetRandomValue(0, 1) == 0 ? -random(1, 5) : random(1, 5);
			sample_noise(src, ctx);
			lines = 0;
			delete line_buffer;
			line_buffer = new std::array<std::pair<Vector2, Vector2>, TOTAL>;
		}

		for(size_t i = 0; i < lines && lines < TOTAL; i++)
		{
			DrawLineV(line_buffer->at(i).first, line_buffer->at(i).second, GREEN);
		}
	}
	else
	{
		//draw_inside(src);
		march_squares(src);
		draw_points(src);
	}
}

void Game::Update()
{
	if constexpr(!VISUALISE)
		sample_noise(src, ctx);
}