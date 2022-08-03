#include "CollisionManager.h"
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;
	}

	Vector3 colliderAPos = colliderA->GetWorldPosition(),
		colliderBPos = colliderB->GetWorldPosition();

	Vector3 vecAtoB = colliderAPos - colliderBPos;
	float disAtoB = vecAtoB.length();

	if (disAtoB <= colliderA->GetRadius() + colliderB->GetRadius())
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollisions(Player* player, Enemy* enemy)
{
	colliders_.clear();

	colliders_.push_back(player);
	colliders_.push_back(enemy);
	for (const std::unique_ptr<PlayerBullet>& bullet : player->bullets_) { colliders_.push_back(bullet.get()); }
	for (const std::unique_ptr<EnemyBullet>& bullet : enemy->missiles_) { colliders_.push_back(bullet.get()); }
	colliders_.push_back(enemy->bomb_.get());

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}