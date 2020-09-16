#pragma once
#include "glm/glm.hpp"
#include "Shapes.h"

class physObject
{
	//forces 
	glm::vec2 forces;

public:

	//poistion
	glm::vec2 pos;
	//velocity
	glm::vec2 vel;
	//mass
	float mass;
	float drag;
	
	collider shape;

	physObject();

	void tickPhysics(float deltaTime);

	void draw() const;

	void addForce(glm::vec2 force);

	void addImpulse(glm::vec2 impulse);

	void addAccel(glm::vec2 accel, float deltaTime);

	void addVelocityChange(glm::vec2 velChange);

	// onCollisionEnter
	// onCollisionStay
	// onCollisionExit
};

