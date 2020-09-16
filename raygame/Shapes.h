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
	glm::vec2 halfExtents;
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
bool checkAabbAabb(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB);
bool checkAabbAabb(glm::vec2 posA, collider aabbA, glm::vec2 posB, collider aabbB);

glm::vec2 gatherCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen);
glm::vec2 gatherCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float& pen);
glm::vec2 gatherCircleAABB(glm::vec2 posA, circle circle, glm::vec2 posB, aabb aabb, float& pen);
glm::vec2 gatherCircleAABB(glm::vec2 posA, collider circle, glm::vec2 posB, collider aabb, float& pen);
glm::vec2 gatherAabbAabb(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB, float& pen);
glm::vec2 gatherAabbAabb(glm::vec2 posA, collider aabbA, glm::vec2 posB, collider aabbB, float& pen);

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float& massA, 
					  glm::vec2 posB, glm::vec2 velB, float& massB, 
					  float elasticity, glm::vec2 colNormal, glm::vec2* dist);