/**
 * @file StageManager.cpp
 * @brief ステージマネージャの実装
 */
#include "stdafx.h"
#include "StageManager.h"
#include "Player.h"
#include "Enemy/EnemyManager.h"
#include "Athletic/AthleticTrigger.h"
#include "Athletic/AthleticStage.h"

StageManager* StageManager::m_instance = nullptr;

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

/**
 * @brief 更新処理
 * @details プレイヤーと各種トリガー（敵生成、ステージ生成、アスレチック生成）の接触判定を行い、
 * ヒットした場合は対応するオブジェクト生成処理を実行します。
 * 生成後はトリガーを削除して二重生成を防ぎます。
 */
void StageManager::Update()
{
	Player* Spawn = FindGO<Player>("player");
	if (!Spawn) return;

	// ▼▼▼ 1. 敵の生成処理 ▼▼▼
	if (m_spawnCollisionObject && m_spawnCollisionObject->IsHit(Spawn->characterController))
	{
		// ボス敵の生成（座標は固定値）
		Vector3 pos = { 3500.0f, -100.0f, -4200.0f };
		EnemyManager::GetInstance()->CreateEnemy(pos);

		// トリガー削除
		DeleteGO(m_spawnCollisionObject);
		m_spawnCollisionObject = nullptr;
	}

	// ▼▼▼ 2. ステージ（特定ギミック）生成処理 ▼▼▼
	if (m_stageCollisionObject && m_stageCollisionObject->IsHit(Spawn->characterController))
	{
		// 既存のを探して、新しく作り直す処理？（実装意図要確認）
		AthleticStage* bg = FindGO<AthleticStage>("athleticStage");
		AthleticStage* newStage = NewGO<AthleticStage>(0, "athleticStage");
		newStage->SetPosition(Vector3(900.0f, -150.0f, -3500.0f));

		DeleteGO(m_stageCollisionObject);
		m_stageCollisionObject = nullptr;
	}

	// ▼▼▼ 3. アスレチックトリガーリストの処理 ▼▼▼
	auto it = m_athleticTriggers.begin();
	while (it != m_athleticTriggers.end())
	{
		if (it->trigger != nullptr && it->trigger->IsHit(Spawn->characterController))
		{
			// リストに登録されている全アスレチック情報を元に生成
			for (const auto& info : it->spawnInfos)
			{
				AthleticStage* newStage = NewGO<AthleticStage>(0, "athleticStage");
				newStage->SetModelPath(info.filePath.c_str());
				newStage->SetPosition(Vector3(900.0f, -150.0f, -3500.0f)); // ※固定座標になっています。動的にする場合は引数等が必要かも

				// 移動パラメータの設定
				if (info.moveSpeed > 0.0f) {
					newStage->SetMoveSettings(info.moveSpeed, info.moveRange);
				}
			}

			// 使用済みトリガーの削除とリストからの消去
			DeleteGO(it->trigger);
			it = m_athleticTriggers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void StageManager::Setup()
{
	// レベルデータ読み込み用ローダーを起動
	NewGO<AthleticTrigger>(0, "athleticTriggerLoader");
}

void StageManager::CreateEnemyBox(Vector3& pos, Quaternion& rot, Vector3& size)
{
	m_spawnCollisionObject = NewGO<CollisionObject>(0);
	m_spawnCollisionObject->CreateBox(pos, rot, size);
	m_spawnCollisionObject->SetIsEnableAutoDelete(false);
	m_spawnCollisionObject->SetName("spawnEnemy");
}

void StageManager::CreateStageBox(Vector3& pos, Quaternion& rot, Vector3& size)
{
	m_stageCollisionObject = NewGO<CollisionObject>(0);
	m_stageCollisionObject->CreateBox(pos, rot, size);
	m_stageCollisionObject->SetIsEnableAutoDelete(false);
	m_stageCollisionObject->SetName("spawnStage");
}

// アスレチック生成用トリガー作成（情報リストを登録）
void StageManager::CreateAthleticBox(Vector3& pos, Quaternion& rot, Vector3& size, const std::vector<AthleticSpawnInfo>& spawnInfos)
{
	CollisionObject* newTrigger = NewGO<CollisionObject>(0);
	newTrigger->CreateBox(pos, rot, size);
	newTrigger->SetIsEnableAutoDelete(false);
	newTrigger->SetName("spawnAthletic");

	AthleticTriggerInfo info;
	info.trigger = newTrigger;
	info.spawnInfos = spawnInfos; // 情報をコピーして保存

	m_athleticTriggers.push_back(info);
}