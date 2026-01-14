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

void StageManager::Update()
{
	Player* Spawn = FindGO<Player>("player");
	if (!Spawn)
	{
		return;
	}

	// ▼▼▼ 1. 敵の生成処理 ▼▼▼
	if (m_spawnCollisionObject && m_spawnCollisionObject->IsHit(Spawn->characterController))
	{
		// 元の座標のまま
		Vector3 pos = { 3500.0f, -100.0f, -4200.0f };
		EnemyManager::GetInstance()->CreateEnemy(pos);
		DeleteGO(m_spawnCollisionObject);
		m_spawnCollisionObject = nullptr;
	}
	// ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	// ▼▼▼ 2. ステージ生成処理▼▼▼
	if (m_stageCollisionObject && m_stageCollisionObject->IsHit(Spawn->characterController))
	{
		AthleticStage* bg = FindGO<AthleticStage>("athleticStage");

		AthleticStage* newStage = NewGO<AthleticStage>(0, "athleticStage");

		newStage->SetPosition(Vector3(900.0f, -150.0f, -3500.0f));

		DeleteGO(m_stageCollisionObject);
		m_stageCollisionObject = nullptr;
	}

	// ▼▼▼ 3. アスレチックトリガーによる生成処理（リスト） ▼▼▼
	auto it = m_athleticTriggers.begin();
	while (it != m_athleticTriggers.end())
	{
		if (it->trigger != nullptr && it->trigger->IsHit(Spawn->characterController))
		{
			// 登録されている情報の数だけループして生成
			for (const auto& info : it->spawnInfos)
			{
				AthleticStage* newStage = NewGO<AthleticStage>(0, "athleticStage");
				newStage->SetModelPath(info.filePath.c_str());

				newStage->SetPosition(Vector3(900.0f, -150.0f, -3500.0f));

				// 速度が設定されていたらパラメータを渡す
				if (info.moveSpeed > 0.0f) {
					newStage->SetMoveSettings(info.moveSpeed, info.moveRange);
				}
			}

			// トリガー削除などの後処理
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
	NewGO<AthleticTrigger>(0, "athleticTriggerLoader");

	/*
	Vector3 pos = { 0.0f, 0.0f, 0.0f };
	Quaternion rot = Quaternion::Identity;
	Vector3 size = Vector3::One;
	CreateAthleticBox(pos, rot, size, {
		// パスだけのコンストラクタも有効
		AthleticSpawnInfo("Assets/modelData/Stage/athletic_NormalMoveFloor.tkm"),
		// パラメータ付き
		AthleticSpawnInfo("Assets/modelData/Stage/athletic_add_MoveFloor.tkm", 2.0f, 100.0f)
	});
	*/
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

// 引数を変更した実装
void StageManager::CreateAthleticBox(Vector3& pos, Quaternion& rot, Vector3& size, const std::vector<AthleticSpawnInfo>& spawnInfos)
{
	CollisionObject* newTrigger = NewGO<CollisionObject>(0);
	newTrigger->CreateBox(pos, rot, size);
	newTrigger->SetIsEnableAutoDelete(false);
	newTrigger->SetName("spawnAthletic");

	AthleticTriggerInfo info;
	info.trigger = newTrigger;
	info.spawnInfos = spawnInfos; // リストをコピー

	m_athleticTriggers.push_back(info);
}