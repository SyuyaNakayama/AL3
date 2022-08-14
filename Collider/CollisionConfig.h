#pragma once

namespace CollisionAttribute
{
	enum
	{
		Player = 0b1,
		PlayerBullet = 0b1 << 1,
		Enemy = 0b1 << 2,
		EnemyMissile = 0b1 << 3,
		EnemyBomb = 0b1 << 4,
		EnemyBeam = 0b1 << 5
	};
}

namespace CollisionMask
{
	enum
	{
		Player = ~(CollisionAttribute::Player | CollisionAttribute::PlayerBullet),
		PlayerBullet = CollisionAttribute::Enemy,
	};
}