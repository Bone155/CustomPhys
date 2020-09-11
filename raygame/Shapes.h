#pragma once

#include <cstdint>

#include "glm/glm.hpp"

// create a struct for every type of shape

struct circle
{
	float radius;
};

// create AABB (axis-aligned bounding box)

struct aabb 
{
	glm::vec2 max;
	glm::vec2 min;
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
		aabb aabbData;
		// add the data for an AABB // done
	};
};

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB);
bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB);
bool checkCircleAABB(glm::vec2 posA, circle circle, glm::vec2 posB, aabb aabb);
bool checkCircleAABB(glm::vec2 posA, collider circle, glm::vec2 posB, collider aabb);
bool checkAabbAabb(aabb aabbA, aabb aabbB);
bool checkAabbAabb(collider aabbA, collider aabbB);
