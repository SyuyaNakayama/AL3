#pragma once
#include <list>
#include "Collider.h"

class CollisionManager
{
private:
	std::list<Collider*> colliders_;
public:
	void CheckAllCollisions()
};