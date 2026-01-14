#include "stdafx.h"
#include "EnemyController.h"
#include "Enemy/NoobEnemy.h"


namespace
{
	const int MOVE_POSITON_NUM = 4;

	struct MovePositionInfo
	{
		Vector3 m_target[MOVE_POSITON_NUM];
		//
		MovePositionInfo(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
		{
			m_target[0] = a;
			m_target[1] = b;
			m_target[2] = c;
			m_target[3] = d;
		}
	};


	static MovePositionInfo MOVE_POSITION_LIST[] = {
		MovePositionInfo(Vector3(3200.0f, -450.0f, -4000.0f), Vector3(3300.0f, -450.0f, -4000.0f), Vector3(3300.0f, -450.0f, -3900.0f), Vector3(3200.0f, -450.0f, -4000.0f)),	// TypeA
		MovePositionInfo(Vector3(3400.0f, -450.0f, -3200.0f), Vector3(3500.0f, -450.0f, -3200.0f), Vector3(3500.0f, -450.0f, -3100.0f), Vector3(3400.0f, -450.0f, -3200.0f)),	// TypeB
		MovePositionInfo(Vector3(3600.0f, -450.0f, -3700.0f), Vector3(3700.0f, -450.0f, -3700.0f), Vector3(3700.0f, -450.0f, -3600.0f), Vector3(3600.0f, -450.0f, -3700.0f)),	// TypeC
		MovePositionInfo(Vector3(3800.0f, -450.0f, -3500.0f), Vector3(3900.0f, -450.0f, -3500.0f), Vector3(3900.0f, -450.0f, -3400.0f), Vector3(3800.0f, -450.0f, -3500.0f)),	// TypeD
		MovePositionInfo(Vector3(4000.0f, -450.0f, -3200.0f), Vector3(4100.0f, -450.0f, -3200.0f), Vector3(4100.0f, -450.0f, -3100.0f), Vector3(4000.0f, -450.0f, -3200.0f)),	// TypeE
		MovePositionInfo(Vector3(4000.0f, -450.0f, -3600.0f), Vector3(4100.0f, -450.0f, -3600.0f), Vector3(4100.0f, -450.0f, -3500.0f), Vector3(4000.0f, -450.0f, -3600.0f)),	// TypeF
		MovePositionInfo(Vector3(3800.0f, -450.0f, -3500.0f), Vector3(3900.0f, -450.0f, -3500.0f), Vector3(3900.0f, -450.0f, -3600.0f), Vector3(3800.0f, -450.0f, -3700.0f)),	// TypeG
		MovePositionInfo(Vector3(3400.0f, -450.0f, -3500.0f), Vector3(3500.0f, -450.0f, -3500.0f), Vector3(3500.0f, -450.0f, -3600.0f), Vector3(3400.0f, -450.0f, -3700.0f)),	// TypeH
		MovePositionInfo(Vector3(3600.0f, -450.0f, -3200.0f), Vector3(3700.0f, -450.0f, -3200.0f), Vector3(3700.0f, -450.0f, -3300.0f), Vector3(3600.0f, -450.0f, -3400.0f)),	// TypeI
		MovePositionInfo(Vector3(3200.0f, -450.0f, -3500.0f), Vector3(3300.0f, -450.0f, -3500.0f), Vector3(3300.0f, -450.0f, -3600.0f), Vector3(3200.0f, -450.0f, -3700.0f)),	// TypeJ
		MovePositionInfo(Vector3(4000.0f, -450.0f, -3400.0f), Vector3(4100.0f, -450.0f, -3400.0f), Vector3(4100.0f, -450.0f, -3500.0f), Vector3(4000.0f, -450.0f, -3600.0f)),	// TypeK
		MovePositionInfo(Vector3(3900.0f, -450.0f, -3300.0f), Vector3(4000.0f, -450.0f, -3300.0f), Vector3(4000.0f, -450.0f, -3400.0f), Vector3(3900.0f, -450.0f, -3500.0f)),	// TypeL
	};
}


std::vector<std::unique_ptr<EnemyController::StateFunc>> EnemyController::m_stateFuncList;


EnemyController::EnemyController()
{

}


EnemyController::~EnemyController()
{

}


bool EnemyController::Start()
{
	return true;
}


void EnemyController::Update()
{
	auto& stateFunc = m_stateFuncList[m_currentState];
	if (stateFunc->m_check(this)) {
		stateFunc->m_exit(this);

		auto& nextStateFunc = m_stateFuncList[m_requestState];
		nextStateFunc->m_enter(this);

		m_currentState = m_requestState;
		return;
	}
	stateFunc->m_update(this);
}


void EnemyController::Render(RenderContext& rc)
{
	// 描画はしない
}

void EnemyController::SetEnemyType(EnEnemyType type)
{
	m_enemyType = type;
	m_target->SetPosition(MOVE_POSITION_LIST[m_enemyType].m_target[0]);
	
}




void EnemyController::Initialize()
{
	// Enumで書いている順番通りに追加してください
	AddStateFunc(IdleEnter, IdleUpdate, IdleExit, IdleCheck);
	AddStateFunc(MoveEnter, MoveUpdate, MoveExit, MoveCheck);
	AddStateFunc(SarchMoveTargetEnter, SarchMoveTargetUpdate, SarchMoveTargetExit, SarchMoveTargetCheck);
}


void EnemyController::IdleEnter(EnemyController* enemy)
{
	// 特に何もしない
}


void EnemyController::IdleUpdate(EnemyController* enemy)
{
	// 特に何もしない
}


void EnemyController::IdleExit(EnemyController* enemy)
{
	// 特に何もしない
}


bool EnemyController::IdleCheck(EnemyController* enemy)
{
	enemy->m_requestState = enAIType_SarchTargetMove;
	return true;
}




void EnemyController::MoveEnter(EnemyController* enemy)
{
	// 特になし
}


void EnemyController::MoveUpdate(EnemyController* enemy)
{
	Vector3 direction = enemy->m_targetPosition - enemy->m_target->GetPosition();
	direction.Normalize();

	enemy->m_target->SetMoveVector(direction);
	enemy->m_target->SetMovePower(1.0f);
}


void EnemyController::MoveExit(EnemyController* enemy)
{
	enemy->m_target->SetMovePower(0.0f);
}


bool EnemyController::MoveCheck(EnemyController* enemy)
{
	Vector3 direction = enemy->m_targetPosition - enemy->m_target->GetPosition();
	if (direction.Length() < 30.0f) {
		enemy->m_requestState = enAIType_SarchTargetMove;
		return true;
	}
	return false;
}




void EnemyController::SarchMoveTargetEnter(EnemyController* enemy)
{
	const auto& moveInfo = MOVE_POSITION_LIST[enemy->m_enemyType];
	const Vector3 targetPosition = moveInfo.m_target[enemy->m_moveIndex];
	enemy->m_moveIndex++;
	if (enemy->m_moveIndex >= MOVE_POSITON_NUM) {
		enemy->m_moveIndex = 0;
	}
	enemy->m_targetPosition = targetPosition;
}


void EnemyController::SarchMoveTargetUpdate(EnemyController* enemy)
{

}


void EnemyController::SarchMoveTargetExit(EnemyController* enemy)
{

}


bool EnemyController::SarchMoveTargetCheck(EnemyController* enemy)
{
	enemy->m_requestState = enAIType_Move;
	return true;
}