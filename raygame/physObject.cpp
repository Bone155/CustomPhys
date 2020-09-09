#include "physObject.h"
#include "raylib.h"
physObject::physObject()
{
	pos = glm::vec2{0,0};
	vel = glm::vec2{ 0,0 };
	forces = glm::vec2{ 0,0 };

	mass = 1.0f;
	shape = { shapeType::CIRCLE, circle{10.0f} };
}

void physObject::tickPhysics(float deltaTime)
{
	vel += forces * deltaTime;
	forces = { 0,0 };
	// implement linear drag
	pos += vel * deltaTime;
}

void physObject::draw() const
{
	switch (shape.colliderShape)
	{
	case shapeType::NONE:
		DrawPixel((int)pos.x, (int)pos.y, RED);
	case shapeType::CIRCLE:
		DrawCircleLines((int)pos.x, (int)pos.y, shape.circleData.radius, RED);
	//case shapeType::AABB:
	//	assert(false && "AABB not yet implemented");
	default:
		break;
	}
	
}

void physObject::addForce(glm::vec2 force)
{
	forces += force / mass;
}

void physObject::addImpulse(glm::vec2 impulse)
{
	vel += impulse / mass;
}

void physObject::addAccel(glm::vec2 accel)
{

}

void physObject::addVelocityChange(glm::vec2 velChang)
{

}
