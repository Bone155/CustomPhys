#include "Game.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

#include "raylib.h"

#include "enumUtils.h"

// define a function that will initialize our static variable in game
collisionMap setupCollisionChecks() {
	collisionMap map;

	map[(collisionPair)(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	map[(collisionPair)(shapeType::CIRCLE | shapeType::AABB)] = checkCircleAABB;
	map[(collisionPair)(shapeType::AABB | shapeType::AABB)] = checkAabbAabb;
	// add CIRCLE-AABB check
	// add AABB-AABB check

	return map;
}

gatherMap setupGatherFunctions() {
	gatherMap map;

	map[(collisionPair)(shapeType::CIRCLE | shapeType::CIRCLE)] = gatherCircleCircle;
	map[(collisionPair)(shapeType::CIRCLE | shapeType::AABB)] = gatherCircleAABB;
	map[(collisionPair)(shapeType::AABB | shapeType::AABB)] = gatherAabbAabb;

	return map;
}

// define the static variable in game
collisionMap Game::collisionCheckers = setupCollisionChecks();
gatherMap Game::gatherFunctions = setupGatherFunctions();

Game::Game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
	useGravity = true;
	srand(time(0));
}

void Game::init()
{
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Custom Physics Simulation");

	SetTargetFPS(60);
}

bool Game::tick()
{
	accumulatedDeltaTime += GetFrameTime();

	if (IsMouseButtonPressed(0)) {
		auto cursorPos = GetMousePosition();

		physObject spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.shape = { shapeType::CIRCLE };
		spawn.shape.circleData.radius = (rand() % 10) + 5.0f;
		spawn.mass = spawn.shape.circleData.radius;

		physobjects.push_back(spawn);
	}

	if (IsMouseButtonPressed(2)) {
		auto cursorPos = GetMousePosition();
		
		physObject spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.shape = { shapeType::AABB };
		spawn.shape.aabbData.halfExtents = { (float)(rand() % 10) + 5.0f, (float)(rand() % 10) + 5.0f };
		spawn.mass = spawn.shape.aabbData.halfExtents.x + spawn.shape.aabbData.halfExtents.y;

		physobjects.push_back(spawn);
	}

	// right-click and push all of the nearby particles within a radius // done
	if (IsMouseButtonPressed(1)) {
		auto cursorPos = GetMousePosition();
		glm::vec2 curs = { cursorPos.x, cursorPos.y };
		for (auto& obj : physobjects) {
			if (glm::distance(glm::vec2{ cursorPos.x, cursorPos.y }, obj.pos) < 75) {
				glm::vec2 direction = glm::normalize(glm::vec2{ obj.pos.x - cursorPos.x , obj.pos.y - cursorPos.y });
				direction.x *= 150;
				direction.y *= 150;
				obj.addImpulse(direction);
			}
		}

	}

	return !WindowShouldClose();
}

void Game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	// test for collision
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
			collisionPair pairType = (collisionPair)(lhs.shape.colliderShape | rhs.shape.colliderShape);

			// the collision check function to call and call it
			bool collision = collisionCheckers[pairType](first->pos, first->shape, second->pos, second->shape);

			if (collision) {
				float pen = 0.0f;
				glm::vec2 normal = gatherFunctions[pairType](first->pos, first->shape, second->pos, second->shape, pen);

				glm::vec2 resImpulses[2];
				resolveCollision(first->pos, first->vel, first->mass,
					second->pos, second->vel, second->mass,
					1.0f, normal, resImpulses);

				first->pos += normal * pen;
				second->pos -= normal * pen;

				first->vel = resImpulses[0];
				second->vel = resImpulses[1];
			}
			
		}

	}
	
	// add gravity for all physics objects // done

	for (auto& obj : physobjects) {
		obj.tickPhysics(fixedTimeStep);
		if (useGravity)
			obj.addAccel({ 0, 9.8 }, fixedTimeStep);
		if (obj.pos.x > (float)GetScreenWidth()) {
			obj.pos.x = 0;
		}
		if (obj.pos.y > (float)GetScreenHeight()) {
			obj.pos.y = 0;
		}
		if (obj.pos.x < 0) {
			obj.pos.x = (float)GetScreenWidth();
		}
		if (obj.pos.y < 0) {
			obj.pos.y = (float)GetScreenHeight();
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
