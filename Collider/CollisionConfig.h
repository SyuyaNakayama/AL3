#pragma once

enum CollisionAttribute
{
	CPlayer = 0b1,
	CPlayerBullet = 0b1 << 1,
	CEnemy = 0b1 << 2,
	CEnemyMissile = 0b1 << 3,
	CEnemyBomb = 0b1 << 4
};

enum CollisionMask
{
	CPlayerMask = ~(CPlayer | CPlayerBullet),
	CEnemyMask = ~(CEnemy | CEnemyMissile | CEnemyBomb)
};