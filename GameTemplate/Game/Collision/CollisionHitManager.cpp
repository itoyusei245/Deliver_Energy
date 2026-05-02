/**
 * @file CollisionHitManager.cpp
 * @brief 衝突判定管理の実装
 */
#include "stdafx.h"
#include "CollisionHitManager.h"
#include "Player.h"
#include "Enemy/Familiar.h"
#include "Athletic/AthleticManager.h"

namespace {
	constexpr const char* NAME_PLAYER			 = "player";	
	constexpr const char* NAME_FAMILIAR			 = "familiar";	
	constexpr int		  DAMAGE_STOMP			 = 1;			
	constexpr float		  STOMP_ALLOWABLE_HEIGHT = 20.0f;		
}

CollisionHitManager* CollisionHitManager::m_instance = nullptr;

/**
 *  衝突判定の更新処理
 */
void CollisionHitManager::Update()
{
	// プレイヤーの取得
	Player* player = FindGO<Player>(NAME_PLAYER);
	if (!player) return;

	// 敵（Familiar）の取得
	Familiar* familiar = FindGO<Familiar>(NAME_FAMILIAR);

	//---------------------------------------------------------------------------------------
	// PlayerとFamiliarの当たり判定処理
	//---------------------------------------------------------------------------------------

// 敵とプレイヤーが接触しているか確認
	if (familiar != nullptr && familiar->GetCollisionObject()->IsHit(player->characterController)) {
		// プレイヤーが落下中であること
		if (player->moveSpeed.y < 0.0f)
		{
			// プレイヤーの「足元」が、敵の「頭上」より高い位置にあること
			float playerFootY = player->position.y;
			float enemyHeadY = familiar->GetPosition().y + STOMP_ALLOWABLE_HEIGHT;

			// 足元が敵の頭より高ければ、踏みつけ成功
			if (playerFootY >= enemyHeadY) {

				// 敵にダメージを与える
				familiar->GetStatus()->Damage(DAMAGE_STOMP);
				player->moveSpeed.y = 300.0f;
			}
			else {
			}
		}
		else {
		}
	}


	//----------------------------------------------------------------------------------------
	// PlayerとAthleticTriggerの当たり判定処理
	//----------------------------------------------------------------------------------------

	//AthleticManagerが存在している時だけ処理を行う
	if (AthleticManager::GetInstance() != nullptr)
	{
		// AthleticManagerからトリガーオブジェクトを取得
		CollisionObject* trigger = AthleticManager::GetInstance()->GetStageTrigger();

		// トリガーが存在し、プレイヤーが接触した場合
		if (trigger != nullptr) {
			if (trigger->IsHit(player->characterController)) {
				// アスレチックステージを生成する
				AthleticManager::GetInstance()->SpawnAthleticStage();

				// トリガー自体は役割を終えたので削除する
				AthleticManager::GetInstance()->DeleteStageTrigger();
			}
		}
	}
}