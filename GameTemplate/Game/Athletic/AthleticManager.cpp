#include "stdafx.h"
#include "Athletic/AthleticManager.h"
#include "Athletic/AthleticTrigger.h"

AthleticManager* AthleticManager::m_instance = nullptr;

AthleticManager::AthleticManager()
{
}

AthleticManager::~AthleticManager()
{
}

void AthleticManager::Setup()
{
	NewGO<AthleticTrigger>(0, "athleticTriggerLoader");
}

void AthleticManager::CreateStageTrigger(const Vector3& pos, const Quaternion& rot, const Vector3& scale) 
{
	if (m_stageTrigger != nullptr)return;

	m_stageTrigger = NewGO<CollisionObject>(0);
	m_stageTrigger->CreateBox(pos, rot, scale);
	m_stageTrigger->SetName("stageTrigger");
}

void AthleticManager::DeleteStageTrigger()
{
	if (m_stageTrigger) {
			DeleteGO(m_stageTrigger);
			m_stageTrigger = nullptr;
	}
}

void AthleticManager::SpawnAthleticStage()
{
	if (m_currentStage != nullptr)return;
	m_currentStage = NewGO<AthleticStage>(0, "athleticStage");
}