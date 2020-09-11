#include "Game.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

#include "raylib.h"

#include "enumUtils.h"

// define a function that will initialize our static variable in game
collisionMap setupCollisionChecks() {
	collisionMap map;

	map[collisionPair(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	
	// add CIRCLE-AABB check
	// add AABB-AABB check

	return map;
}

// define the static variable in game
collisionMap Game::collisionCheckers = setupCollisionChecks();

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

		physObject spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.shape = { shapeType::CIRCLE, circle{10.0f} };
		spawn.mass = (rand() % 10) + 1;
		spawn.shape.circleData.radius = spawn.mass;
		spawn.addImpulse({ 100, 0 });

		physobjects.push_back(spawn);
	}
	if (IsMouseButtonPressed(2)) {
		auto cursorPos = GetMousePosition();
		glm::vec2 max = { 10.0f, 10.0f };
		glm::vec2 min = { 1.0f, 1.0f };
		physObject spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		//spawn.shape = { shapeType::AABB, aabb{ max, min } }; // ask terry for help
		spawn.mass = (rand() % 10) + 1;
		spawn.addImpulse({ 100, 0 });

		physobjects.push_back(spawn);
	}
	// right-click and push all of the nearby particles within a radius
	if (IsMouseButtonPressed(1)) {
		auto cursorPos = GetMousePosition();
		glm::vec2 curs = { cursorPos.x, cursorPos.y };
		for (auto& obj : physobjects) {
			float distance = glm::length(curs - obj.pos);
			if (distance <= 15) {
				obj.addForce({ 0, -100 });
			}
		}

		/*float radius = 15;
		for (auto& obj : physobjects) {
			glm::vec2 distance = {(obj.pos.x - cursorPos.x),(obj.pos.y - cursorPos.y)};
			if (distance.x <= radius) {
				obj.addForce({ -25, 0 });
			}
			if (distance.y <= radius) {
				obj.addForce({ 0, -225 });
			}
		}*/
	}

	return !WindowShouldClose();
}

void Game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;


	// test for collision
	// optimize with spatial partioning
	for (auto& lhs : physobjects) {
		for (auto& rhs : physobjects) {
			if (&lhs == &rhs) { continue; }

			auto *first = &lhs;
			auto *second = &rhs;

			if (static_cast<uint8_t>(lhs.shape.colliderShape) >
				static_cast<uint8_t>(rhs.shape.colliderShape)) {
				first = &rhs;
				second = &lhs;
			}
			// the type of collision to test for
			collisionPair pairType = collisionPair(lhs.shape.colliderShape | rhs.shape.colliderShape);

			// the collision check function to call and call it
			bool collision = collisionCheckers[pairType](first->pos, first->shape, second->pos, second->shape);

			if (collision) {
				std::cout << "collision" << std::endl;
			}
			
		}
	}
	
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
