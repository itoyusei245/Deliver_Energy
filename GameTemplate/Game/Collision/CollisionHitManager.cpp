/**
 * @file CollisionHitManager.cpp
 * @brief 衝突判定管理の実装
 */
#include "stdafx.h"
#include "CollisionHitManager.h"
#include "Player.h"
#include "Enemy/Familiar.h"
#include "Athletic/AthleticManager.h"

CollisionHitManager* CollisionHitManager::m_instance = nullptr;

/**
 * @brief 衝突判定の更新処理
 * @details 以下の判定を行います。
 * 1. PlayerとFamiliar（敵）: 踏みつけ判定を行い、成功すれば敵にダメージを与えます。
 * 2. PlayerとAthleticTrigger: アスレチックステージの開始トリガーを検知します。
 */
void CollisionHitManager::Update()
{
	// プレイヤーの取得
	Player* player = FindGO<Player>("player");
	if (!player) return;

	// 敵（Familiar）の取得
	// FIXME: 敵がいない場合(nullptr)のチェックが必要かもしれません
	Familiar* familiar = FindGO<Familiar>("familiar");

	//---------------------------------------------------------------------------------------
	// PlayerとFamiliarの当たり判定処理
	//---------------------------------------------------------------------------------------

	// 敵とプレイヤーが接触しているか確認
	if (familiar->GetCollisionObject()->IsHit(player->characterController)) {

		// プレイヤーから敵へのベクトルではなく、ここでは「位置関係のベクトル」を計算
		// vec: プレイヤー位置 -> 敵位置 のベクトル
		Vector3 vec = familiar->GetPosition() - player->position;
		vec.Normalize();

		// ベクトルと上方向(Vector3::Up)の内積を取る
		// これにより、敵がプレイヤーの真下に近いほど、shetaは 1.0 に近づく
		const float sheta = vec.Dot(Vector3::Up);

		// 内積から角度（ラジアン）を算出
		const float angle = acosf(sheta);

		// 角度が15度未満の場合（プレイヤーのほぼ真下に敵がいる場合）
		// つまり「プレイヤーが敵を踏みつけた」と判定
		if (angle < 15.0f) { // 15度を閾値とする（Math::DegToRadを使っている場合は注意）
			int test = 0; // デバッグ用変数？
			// 敵にダメージを与える
			familiar->GetStatus()->Damage(1);
		}
	}


	//----------------------------------------------------------------------------------------
	// PlayerとAthleticTriggerの当たり判定処理
	//----------------------------------------------------------------------------------------

	{
		// AthleticManagerからトリガーオブジェクトを取得
		CollisionObject* trigger = AthleticManager::GetInstance()->GetStageTrigger();

		// トリガーが存在し、プレイヤーが接触した場合
		if (trigger != nullptr) {
			if (trigger->IsHit(player->characterController)) {
				// アスレチックステージを生成（スポーン）する
				AthleticManager::GetInstance()->SpawnAthleticStage();

				// トリガー自体は役割を終えたので削除する
				AthleticManager::GetInstance()->DeleteStageTrigger();
			}
		}
	}
}