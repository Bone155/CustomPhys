#pragma once

#include <cstdint>

// create a struct for every type of shape

struct circle
{
	float radius;
};

// create AABB (axis-aligned bounding box)

struct AABB 
{
	glm::vec2 center;
};

enum class shapeType : uint8_t
{
	NONE	= 0,
	CIRCLE	= 1 << 0,
	AABB	= 1 << 1 // implement AABBs into shape system
};
// shapeType::CIRCLE

struct collider
{
	shapeType colliderShape;
	union 
	{
		circle circleData;
		// add the data for an AABB
	};
};


