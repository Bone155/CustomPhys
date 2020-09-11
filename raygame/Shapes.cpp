#include "Shapes.h"
#include "glm/glm.hpp"
bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB)
{
	// distance between two circles
	float distance = glm::length(posA - posB);
	// glm::distance works as well

	// sum of radii
	float sum = circleA.radius + circleB.radius;

	return distance < sum;
}

bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB)
{
	return checkCircleCircle(posA, circleA.circleData, posB, circleB.circleData);
}

bool checkCircleAABB(glm::vec2 posA, circle circle, glm::vec2 posB, aabb aabb)
{
	float distance = glm::length(posA - posB);
	float sum = circle.radius * circle.radius;
	return distance < sum;
}

bool checkCircleAABB(glm::vec2 posA, collider circle, glm::vec2 posB, collider aabb)
{
	return checkCircleAABB(posA, circle.circleData, posB, aabb.aabbData);
}

bool checkAabbAabb(aabb aabbA, aabb aabbB)
{
	return !(aabbB.max.x < aabbA.min.x || aabbA.max.x < aabbB.min.x || aabbB.max.y < aabbA.min.y || aabbA.max.y < aabbB.min.y);
}

bool checkAabbAabb(collider aabbA, collider aabbB)
{
	return checkAabbAabb(aabbA.aabbData, aabbB.aabbData);
}
