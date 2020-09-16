#include "physObject.h"
#include "raylib.h"
physObject::physObject()
{
	pos = glm::vec2{0,0};
	vel = glm::vec2{ 0,0 };
	forces = glm::vec2{ 0,0 };
	shape.colliderShape = shapeType::NONE;
	mass = 1.0f;
}

void physObject::tickPhysics(float deltaTime)
{
	vel += forces * deltaTime;
	forces = { 0,0 };
	// implement linear drag // ask terry about linear drag
	pos += vel * deltaTime;
}

void physObject::draw() const
{
	switch (shape.colliderShape)
	{
	case shapeType::NONE:
		DrawPixel((int)pos.x, (int)pos.y, RED);
		break;
	case shapeType::CIRCLE:
		DrawCircleLines((int)pos.x, (int)pos.y, shape.circleData.radius, RED);
		break;
	case shapeType::AABB:
		// implement draw AABB // done
		glm::vec2 min = { pos.x, pos.y };
		glm::vec2 max = { pos.x + (shape.aabbData.width), pos.y - (shape.aabbData.height) };
		DrawRectangleLines(pos.x, pos.y, shape.aabbData.width, shape.aabbData.height, BLUE);
		DrawCircle(min.x, min.y, 5, GREEN);
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

void physObject::addAccel(glm::vec2 accel, float deltaTime)
{
	vel += accel * deltaTime;
}

void physObject::addVelocityChange(glm::vec2 velChang)
{
	vel += velChang;
}
