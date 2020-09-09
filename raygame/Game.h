#pragma once
#include <vector>
#include "physObject.h"
class Game
{
	float accumulatedDeltaTime;
	std::vector<physObject> physobjects;
public:
	float fixedTimeStep;

	Game();

	void init();

	bool tick();
	
	void tickPhysics();

	bool shouldTickPhysics() const;

	void draw() const;

	void exit();
};

