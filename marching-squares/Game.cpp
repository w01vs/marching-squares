#include "Game.h"

Game::Game(const int width, const int height, int fps, std::string title)
{
	assert(!GetWindowHandle());
	InitWindow(width, height, title.c_str());
	Init();
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
	src = {gen_source() };
}

void Game::Tick()
{
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

void Game::Draw() const
{
	ClearBackground(BLACK);
	draw_inside(src);
	march_squares(src);
	//draw_points(src);
	
}

void Game::Update()
{
	sample_noise(src);
}