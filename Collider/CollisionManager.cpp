#include "CollisionManager.h"

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
	{
		return;
	}

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAB = vecAB.abs();

	if (vecAB <= radAB)
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
	colliders_.push_back(enemy);
	for (std::unique_ptr<EnemyBullet>& bullet : enemy->missiles_) { colliders_.push_back(bullet.get()); }
	for (std::unique_ptr<Bomb>& bomb : enemy->bomb_) { colliders_.push_back(bomb.get()); }
	if (enemy->isStart) { for (size_t i = 0; i < enemy->beam_.size(); i++) { colliders_.push_back(&enemy->beam_[i]); } }

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		if (*itrA == enemy) { break; }
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			CheckCollisionPair(*itrA, *itrB);
		}
	}

	if (enemy->isRippleExist)
	{
		Vector3 vecAB = player->GetWorldPosition() - enemy->rippleTransform_.translation_;
		vecAB.y = 0;
		float dis = vecAB.length();
		float radAB = player->GetRadius().x + enemy->rippleTransform_.scale_.x;

		if (player->GetWorldPosition().y == 0 && dis <= radAB && dis >= radAB - 15.0f) { player->OnCollision(); }
	}
}