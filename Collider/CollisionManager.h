#pragma once
#include <list>
#include "Collider.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include <memory>


class CollisionManager
{
private:
	bool CheckCollisionPair(Collider* colliderA, Collider* colliderB);
public:
	void CheckAllCollisions(Player* player, Enemy* enemy);
};