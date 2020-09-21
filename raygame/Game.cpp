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
		spawn.shape.circleData.radius = 15.0f;
		spawn.mass = 1.0f;
		spawn.addImpulse({ 150, 0 });

		physobjects.push_back(spawn);
	}

	if (IsMouseButtonPressed(2)) {
		auto cursorPos = GetMousePosition();
		
		physObject spawn;
		spawn.pos = { cursorPos.x, cursorPos.y };
		spawn.shape = { shapeType::AABB };
		spawn.shape.aabbData.halfExtents = { 15, 15 };
		//spawn.shape.aabbData.width = 30;
		//spawn.shape.aabbData.height = 30;
		spawn.mass = 1.0f;
		spawn.addImpulse({ -300, 0 });

		physobjects.push_back(spawn);
	}

	// right-click and push all of the nearby particles within a radius // done
	if (IsMouseButtonPressed(1)) {
		auto cursorPos = GetMousePosition();
		glm::vec2 curs = { cursorPos.x, cursorPos.y };
		for (auto& obj : physobjects) {
			float distance = glm::length(curs - obj.pos);
			if (distance <= 50) {
				obj.vel *= -30;
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
		if (obj.pos.x >= (float)GetScreenWidth()) {
			obj.pos.x = 0;
		}
		if (obj.pos.y >= (float)GetScreenHeight()) {
			obj.pos.y = 0;
		}
		if (obj.pos.x <= 0) {
			obj.pos.x = (float)GetScreenWidth();
		}
		if (obj.pos.y <= 0) {
			obj.pos.y = (float)GetScreenHeight();
		}

		////  check the distance between the mouse cursor and the object
		//if (glm::distance(glm::vec2{ cursorPos.x, cursorPos.y }, targets.pos) < 50)
		//{
		//	//  Push the objects away from the mouse
		//	glm::vec2 direction = glm::normalize(glm::vec2{ targets.pos.x - cursorPos.x , targets.pos.y - cursorPos.y });
		//	direction.x *= 30;
		//	direction.y *= 30;
		//	targets.addImpulse(direction);
		//}
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
