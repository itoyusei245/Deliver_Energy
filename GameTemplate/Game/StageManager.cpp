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

namespace 
{
	constexpr const char* NAME_PLAYER		  = "player";

	constexpr const char* NAME_ATHLETIC_STAGE = "athleticStage";

	constexpr const char* NAME_TRIGGER_LOADER = "athleticTriggerLoader";

	constexpr const char* NAME_SPAWN_ENEMY	  = "spawnEnemy";

	constexpr const char* NAME_SPAWN_STAGE    = "spawnStage";

	constexpr const char* NAME_SPAWN_ATHLETIC = "spawnAthletic";


	const Vector3 POS_BOSS_SPAWN(3500.0f, -100.0f, -4200.0f);

	const Vector3 POS_STAGE_SPAWN(900.0f, -150.0f, -3500.0f);
}

StageManager* StageManager::m_instance = nullptr;

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	// ---------------------------------------------------
	// 自分が生成した全てのアスレチック（床）を一斉削除する
	// ---------------------------------------------------
	for (auto* stage : m_spawnedStages) {
		if (stage != nullptr) {
			DeleteGO(stage);
		}
	}
	m_spawnedStages.clear(); // リストを空にする

	// もし踏まれていないトリガーが残っていたら、それも消す
	if (m_spawnCollisionObject) DeleteGO(m_spawnCollisionObject);
	if (m_stageCollisionObject) DeleteGO(m_stageCollisionObject);
	for (auto& info : m_athleticTriggers) {
		if (info.trigger) DeleteGO(info.trigger);
	}
	m_athleticTriggers.clear();
}

/**
 * @brief 更新処理
 * @details プレイヤーと各種トリガー（敵生成、ステージ生成、アスレチック生成）の接触判定を行い、
 * ヒットした場合は対応するオブジェクト生成処理を実行します。
 * 生成後はトリガーを削除して二重生成を防ぎます。
 */
void StageManager::Update()
{
	Player* Spawn = FindGO<Player>(NAME_PLAYER);
	if (!Spawn) return;

	if (m_spawnCollisionObject && m_spawnCollisionObject->IsHit(Spawn->characterController))
	{
		// ボス敵の生成（座標は固定値）
		Vector3 pos = POS_BOSS_SPAWN;
		EnemyManager::GetInstance()->CreateEnemy(pos);

		// イベント再生をリクエストする
		m_isBossEventRequested = true;

		// トリガー削除
		DeleteGO(m_spawnCollisionObject);
		m_spawnCollisionObject = nullptr;
	}

	if (m_stageCollisionObject && m_stageCollisionObject->IsHit(Spawn->characterController))
	{
		AthleticStage* bg = FindGO<AthleticStage>(NAME_ATHLETIC_STAGE);
		AthleticStage* newStage = NewGO<AthleticStage>(0, NAME_ATHLETIC_STAGE);
		newStage->SetPosition(POS_STAGE_SPAWN);

		m_spawnedStages.push_back(newStage);

		DeleteGO(m_stageCollisionObject);
		m_stageCollisionObject = nullptr;
	}

	auto it = m_athleticTriggers.begin();
	while (it != m_athleticTriggers.end())
	{
		if (it->trigger != nullptr && it->trigger->IsHit(Spawn->characterController))
		{
			// リストに登録されている全アスレチック情報を元に生成
			for (const auto& info : it->spawnInfos)
			{
				AthleticStage* newStage = NewGO<AthleticStage>(0, NAME_ATHLETIC_STAGE);
				newStage->SetModelPath(info.filePath.c_str());
				newStage->SetPosition(POS_STAGE_SPAWN);
				// 移動パラメータの設定
				if (info.moveSpeed > 0.0f) {
					newStage->SetMoveSettings(info.moveSpeed, info.moveRange);
				}

				m_spawnedStages.push_back(newStage);
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
	NewGO<AthleticTrigger>(0, NAME_TRIGGER_LOADER);
}

void StageManager::CreateEnemyBox(Vector3& pos, Quaternion& rot, Vector3& size)
{
	m_spawnCollisionObject = NewGO<CollisionObject>(0);
	m_spawnCollisionObject->CreateBox(pos, rot, size);
	m_spawnCollisionObject->SetIsEnableAutoDelete(false);
	m_spawnCollisionObject->SetName(NAME_SPAWN_ENEMY);
}

void StageManager::CreateStageBox(Vector3& pos, Quaternion& rot, Vector3& size)
{
	m_stageCollisionObject = NewGO<CollisionObject>(0);
	m_stageCollisionObject->CreateBox(pos, rot, size);
	m_stageCollisionObject->SetIsEnableAutoDelete(false);
	m_stageCollisionObject->SetName(NAME_SPAWN_STAGE);
}

// アスレチック生成用トリガー作成（情報リストを登録）
void StageManager::CreateAthleticBox(Vector3& pos, Quaternion& rot, Vector3& size, const std::vector<AthleticSpawnInfo>& spawnInfos)
{
	CollisionObject* newTrigger = NewGO<CollisionObject>(0);
	newTrigger->CreateBox(pos, rot, size);
	newTrigger->SetIsEnableAutoDelete(false);
	newTrigger->SetName(NAME_SPAWN_ATHLETIC);

	AthleticTriggerInfo info;
	info.trigger = newTrigger;
	info.spawnInfos = spawnInfos; // 情報をコピーして保存

	m_athleticTriggers.push_back(info);
}