#include "physObject.h"
#include "raylib.h"
physObject::physObject()
{
	pos = glm::vec2{0,0};
	vel = glm::vec2{ 0,0 };
	forces = glm::vec2{ 0,0 };

	shape.colliderShape = shapeType::NONE;

	mass = 1.0f;
	drag = 1.0f;
}

void physObject::tickPhysics(float deltaTime)
{
	vel += forces * deltaTime;
	forces = { 0,0 };
	
	// implement linear drag // ask terry about linear drag
	vel *= (1.0f - deltaTime * drag);

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
		/*DrawRectangleLines((int)pos.x, (int)pos.y,
						   (int)shape.aabbData.width, (int)shape.aabbData.height, BLUE);*/
		DrawRectangleLines(pos.x - shape.aabbData.halfExtents.x, pos.y - shape.aabbData.halfExtents.y,
						   shape.aabbData.halfExtents.x * 2, shape.aabbData.halfExtents.y * 2, BLUE);
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
	forces += accel;
}

void physObject::addVelocityChange(glm::vec2 velChang)
{
	vel += velChang;
}
