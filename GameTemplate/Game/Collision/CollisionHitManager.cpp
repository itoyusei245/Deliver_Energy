#include "stdafx.h"
#include "CollisionHitManager.h"
#include "Player.h"
#include "Enemy/Familiar.h"


CollisionHitManager* CollisionHitManager::m_instance = nullptr;


void CollisionHitManager::Update()
{
	// PlayerとFamiliarの当たり判定処理
	{
		Player* player = FindGO<Player>("player");
		Familiar* familiar = FindGO<Familiar>("familiar");

		if (familiar->GetCollisionObject()->IsHit(player->characterController)) {
			Vector3 vec = familiar->GetPosition() - player->position;
			vec.Normalize();

			const float sheta = vec.Dot(Vector3::Up);
			const float angle = acosf(sheta);

			if (angle < 15.0f) {
				int test = 0;
				familiar->GetStatus()->Damage(1);
			}
		}
	}
}