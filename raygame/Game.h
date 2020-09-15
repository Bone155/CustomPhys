#pragma once
#include <vector>
#include <unordered_map>

#include "physObject.h"

// function pointer syntax
//				RETURN_TYPE(*)(PARAMETER_TYPES)

// given two objects, we need to figure out what function to call
// in order to test whether they are in collision

using collisionPair = uint8_t; // type-alias, like a typedef

using collisionFunc = bool(*)(glm::vec2, collider, glm::vec2, collider);
using collisionMap = std::unordered_map<collisionPair, collisionFunc>;

using gatherFunc = glm::vec2(*)(glm::vec2, collider, glm::vec2, collider, float&);
using gatherMap = std::unordered_map<collisionPair, gatherFunc>;

class Game
{
	float accumulatedDeltaTime;
	std::vector<physObject> physobjects;

	static collisionMap collisionCheckers;
	static gatherMap gatherFunctions;
public:
	float fixedTimeStep;

	Game();

	void init();

	bool tick();
	
	void tickPhysics();

	bool shouldTickPhysics() const;

	void draw() const;

	void exit();

	// bool shouldClose() const
	// bool shouldPhysics() const

	// float targetFixedStep
	// float gravityScale

	bool useGravity;

};

