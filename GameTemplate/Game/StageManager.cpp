#include "stdafx.h"
#include "StageManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TriggerBox.h"
#include "AthleticStage.h"

StageManager* StageManager ::m_instance= nullptr;

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
	if (m_spawnCollisionObject && m_spawnCollisionObject->IsHit(Spawn->characterController))
	{
		Vector3 pos = { 3500.0f, -100.0f, -4200.0f };
		EnemyManager::GetInstance()->CreateEnemy(pos);
		DeleteGO(m_spawnCollisionObject);
		m_spawnCollisionObject = nullptr;
	}
	//{ 1500.0f, -100.0f, -4200.0f }
	if (m_stageCollisionObject && m_stageCollisionObject->IsHit(Spawn->characterController))
	{
		AthleticStage* bg = FindGO<AthleticStage>("athleticStage");
		NewGO<AthleticStage>(0, "athleticStage");
		DeleteGO(m_stageCollisionObject);
		m_stageCollisionObject = nullptr;
	}
}

void StageManager::Setup()
{
	m_triggerBox = NewGO<TriggerBox>(0, "triggerBox");
	m_triggerBox->Init();

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