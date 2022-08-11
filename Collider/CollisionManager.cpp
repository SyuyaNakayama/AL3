#include "CollisionManager.h"
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;
	}

	Vector3 vecAtoB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAandB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAtoB = vecAtoB.abs();

	if (vecAtoB <= radAandB)
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollisions(Player* player, Enemy* enemy)
{
	std::list<Collider*> colliders_;
	colliders_.push_back(player);
	for (std::unique_ptr<PlayerBullet>& bullet : player->bullets_) { colliders_.push_back(bullet.get()); }
	std::list<Collider*>::iterator enemyCollider = colliders_.end();
	colliders_.push_back(enemy);
	enemyCollider--;
	for (std::unique_ptr<EnemyBullet>& bullet : enemy->missiles_) { colliders_.push_back(bullet.get()); }
	for (std::unique_ptr<Bomb>& bomb : enemy->bomb_) { colliders_.push_back(bomb.get()); }
	if (enemy->isStart) { for (size_t i = 0; i < enemy->beam_.size(); i++) { colliders_.push_back(&enemy->beam_[i]); } }

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		if (itrA == enemyCollider) { break; }
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