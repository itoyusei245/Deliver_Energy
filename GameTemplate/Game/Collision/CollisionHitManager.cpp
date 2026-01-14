#include "stdafx.h"
#include "CollisionHitManager.h"
#include "Player.h"
#include "Enemy/Familiar.h"
#include "Athletic/AthleticManager.h"


CollisionHitManager* CollisionHitManager::m_instance = nullptr;


void CollisionHitManager::Update()
{
	
	Player* player = FindGO<Player>("player");
	if (!player)return;

	Familiar* familiar = FindGO<Familiar>("familiar");
		
	//---------------------------------------------------------------------------------------
	// PlayerとFamiliarの当たり判定処理
	//---------------------------------------------------------------------------------------


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


	//----------------------------------------------------------------------------------------
	//PlayerとAthleticTriggerの当たり判定処理
	//----------------------------------------------------------------------------------------


	{
		CollisionObject* trigger = AthleticManager::GetInstance()->GetStageTrigger();
		if (trigger != nullptr) {
			if (trigger->IsHit(player->characterController)) {
				AthleticManager::GetInstance()->SpawnAthleticStage();
				AthleticManager::GetInstance()->DeleteStageTrigger();
			}
		}
	}
}