#include "physObject.h"
#include "raylib.h"
physObject::physObject()
{
	pos = glm::vec2{0,0};
	vel = glm::vec2{ 0,0 };
	forces = glm::vec2{ 0,0 };

	mass = 1.0f;
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
		break;
	case shapeType::CIRCLE:
		DrawCircleLines((int)pos.x, (int)pos.y, shape.circleData.radius, RED);
		break;
	case shapeType::AABB:
		// implement draw AABB
		DrawLine((int)shape.aabbData.min.x, (int)shape.aabbData.min.y, (int)shape.aabbData.max.x, (int)shape.aabbData.min.y, BLUE);
		DrawLine((int)shape.aabbData.max.x, (int)shape.aabbData.min.y, (int)shape.aabbData.max.x, (int)shape.aabbData.max.y, BLUE);
		DrawLine((int)shape.aabbData.max.x, (int)shape.aabbData.max.y, (int)shape.aabbData.min.x, (int)shape.aabbData.max.y, BLUE);
		DrawLine((int)shape.aabbData.min.x, (int)shape.aabbData.max.y, (int)shape.aabbData.min.x, (int)shape.aabbData.min.y, BLUE);
		//assert(false && "AABB not yet implemented");
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
