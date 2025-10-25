#include "stdafx.h"
#include "StageManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TriggerBox.h"

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
	if (m_collisionObject->IsHit(Spawn->characterController))
	{
		Vector3 pos = { 3500.0f, -100.0f, -4200.0f };
		EnemyManager::GetInstance()->CreateEnemy(pos);
		DeleteGO(m_collisionObject);
	}
}

void StageManager::Setup()
{
	m_triggerBox = NewGO<TriggerBox>(0, "triggerBox");
	m_triggerBox->Init();

}

void StageManager::CreateBox(Vector3& pos, Quaternion& rot, Vector3& size) {
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(pos, rot, size);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("spawnEnemy");

}