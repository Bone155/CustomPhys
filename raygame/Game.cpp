#include "Game.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "raylib.h"

Game::Game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;

	srand(time(0));
}

void Game::init()
{
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
}

bool Game::tick()
{
	accumulatedDeltaTime += GetFrameTime();
	if (IsMouseButtonPressed(0)) {
		auto cursorPos = GetMousePosition();

		physObject baby;
		baby.pos = { cursorPos.x, cursorPos.y };
		baby.mass = (rand() % 10) + 1;
		baby.addImpulse({ 100, 0 });

		physobjects.push_back(baby);
	}
	// right-click and push all of the nearby particles within a radius
	if (IsMouseButtonPressed(1)) {
		auto cursorPos = GetMousePosition();

		for (auto& obj : physobjects) {
			glm::vec2 distance = {(obj.pos.x - cursorPos.x),(obj.pos.y - cursorPos.y)};
			if (distance.x <= 15) {
				obj.addForce({ -25, 0 });
			}
			if (distance.y <= 15) {
				obj.addForce({ 0, -225 });
			}
		}
	}

	return !WindowShouldClose();
}

void Game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;
	// add gravity for all physics objects // done

	for (auto& obj : physobjects) {
		obj.tickPhysics(fixedTimeStep);
		obj.addForce({ 0,9.8 });
		if (obj.pos.x >= GetScreenWidth()) {
			obj.pos.x = 0;
		}
		if (obj.pos.y >= GetScreenHeight()) {
			obj.pos.y = 0;
		}
		if (obj.pos.x <= 0) {
			obj.pos.x = GetScreenWidth();
		}
		if (obj.pos.y <= 0) {
			obj.pos.y = GetScreenHeight();
		}
	}
	// wrap the object the level // done
}

bool Game::shouldTickPhysics() const
{
	return accumulatedDeltaTime >= fixedTimeStep;
}

void Game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto& obj : physobjects) {
		obj.draw();
	}

	EndDrawing();
}

void Game::exit()
{
	CloseWindow();
}
