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
	//  get the disttance between the two shapes
	float distance = glm::length(posA - posB);

	//  get the sum of the radius and hypotenuse
	float sum = circle.radius + sqrt(aabb.halfExtents.x * aabb.halfExtents.x + aabb.halfExtents.y * aabb.halfExtents.y);

	//  return distance < sum
	return (posA.x + aabb.halfExtents.x > posB.x - circle.radius &&
		posA.x - aabb.halfExtents.x < posB.x + circle.radius &&
		posA.y + aabb.halfExtents.y > posB.y - circle.radius &&
		posA.y - aabb.halfExtents.y < posB.y + circle.radius &&
		distance < sum);
}

bool checkCircleAABB(glm::vec2 posA, collider circle, glm::vec2 posB, collider aabb)
{
	return checkCircleAABB(posA, circle.circleData, posB, aabb.aabbData);
}

bool checkAabbAabb(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB)
{
	return (posA.x + aabbA.halfExtents.x > posB.x - aabbB.halfExtents.x &&
		     posA.x - aabbA.halfExtents.x < posB.x + aabbB.halfExtents.x &&
		     posA.y + aabbA.halfExtents.y > posB.y - aabbB.halfExtents.y &&
		     posA.y - aabbA.halfExtents.y < posB.y + aabbB.halfExtents.y);
}

bool checkAabbAabb(glm::vec2 posA, collider aabbA, glm::vec2 posB, collider aabbB)
{
	return checkAabbAabb(posA, aabbA.aabbData, posB, aabbB.aabbData);
}

glm::vec2 gatherCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen)
{
	float dist = glm::length(posA - posB);
	float sum = circleA.radius + circleB.radius;
	// pen is equal the difference between the the sum of their radii sub distance between the two circles
	pen = sum - dist;

	return glm::normalize(posA - posB);
}

glm::vec2 gatherCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float& pen)
{
	return gatherCircleCircle(posA, circleA.circleData, posB, circleB.circleData, pen);
}

glm::vec2 gatherCircleAABB(glm::vec2 posA, circle circle, glm::vec2 posB, aabb aabb, float& pen)
{
	//  calculate their distance
	float dist = glm::length(posA - posB);

	//  check the penetration level and move it based on the most penetrated side
	if (((posA.x + aabb.halfExtents.x) - (posB.x - circle.radius / 2)) > ((posA.y + aabb.halfExtents.y) - (posB.y - circle.radius / 2)))
	{
		float sum = aabb.halfExtents.x + circle.radius;
		pen = sum - dist;
	}
	else
	{
		float sum = aabb.halfExtents.y + circle.radius;
		pen = sum - dist;
	}

	return glm::normalize(posA - posB);
}

glm::vec2 gatherCircleAABB(glm::vec2 posA, collider circle, glm::vec2 posB, collider aabb, float& pen)
{
	return gatherCircleAABB(posA, circle.circleData, posB, aabb.aabbData, pen);
}

glm::vec2 gatherAabbAabb(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB, float& pen)
{
	//  calculate their distance
	float dist = glm::length(posA - posB);

	//  check the penetration level and move it based on the most penetrated side
	if (((posA.x + aabbA.halfExtents.x) - (posB.x - aabbB.halfExtents.x)) > (posA.y + aabbA.halfExtents.y) - (posB.y - aabbB.halfExtents.y))
	{
		float sum = aabbA.halfExtents.x + aabbB.halfExtents.x;
		pen = sum - dist;
	}
	else
	{
		float sum = aabbA.halfExtents.y + aabbB.halfExtents.y;
		pen = sum - dist;
	}

	return glm::normalize(posA - posB);
}

glm::vec2 gatherAabbAabb(glm::vec2 posA, collider aabbA, glm::vec2 posB, collider aabbB, float& pen)
{
	return gatherAabbAabb(posA, aabbA.aabbData, posB, aabbB.aabbData, pen);
}

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float& massA, glm::vec2 posB, glm::vec2 velB, float& massB, float elasticity, glm::vec2 colNormal, glm::vec2* dist)
{
	// calculate relative velocity
	glm::vec2 relVel = velA - velB;

	// calculate impule magnitude
	float impulseMag = glm::dot(-(1.0f + elasticity) * relVel, colNormal) /
					   glm::dot(colNormal, colNormal * (1 / massA + 1 / massB));

	// write out impulses for the two objects into the given array
	dist[0] = velA + (impulseMag / massA) * colNormal;
	dist[1] = velB - (impulseMag / massA) * colNormal;
}
