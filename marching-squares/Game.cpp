#include "Game.h"

Game::Game(const int width, const int height, int fps, const std::string& title, const bool D3)
{
	assert(!GetWindowHandle());
	InitWindow(width, height, title.c_str());
	Init();
	this->D3 = D3;
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
	sample_noise(src);
	camera.position = {0.0f, 0.0f, 0.0f};
	camera.fovy = 45;
	camera.target = {10.0f, 0.0f, 10.0f};
	camera.projection = CAMERA_PERSPECTIVE;
	camera.up = {0.0f, 1.0f, 0.0f};
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
	if(!D3)
	{
		//draw_inside(src);
		march_squares(src);
		draw_points(src);
	}
	else
	{
		BeginMode3D(camera);





		EndMode3D();
	}


}

void Game::Update()
{
	//sample_noise(src);
}