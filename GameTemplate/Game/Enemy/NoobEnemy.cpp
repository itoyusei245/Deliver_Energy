#include "stdafx.h"
#include "NoobEnemy.h"


NoobEnemy::NoobEnemy()
{
	m_status = new NoobEnemyStatus();
	m_status->Setup();
}


NoobEnemy::~NoobEnemy()
{
	delete m_status;
	m_status = nullptr;

	for (auto* state : m_stateList) {
		delete state;
	}
}


bool NoobEnemy::Start()
{
	SetScale(Vector3(70.0f, 70.0f, 70.0f));

	m_stateList[enNoobEnemyStateType_Idle] = new NoobEnemyIdleState(this);
	m_stateList[enNoobEnemyStateType_Move] = new NoobEnemyMoveState(this);
	m_modelRender.Init("Assets/animData/main_bossEnemy.tkm");
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	
	m_collisionObject.CreateSphere(m_position, m_rotation, 10.0f);
	m_collisionObject.Update();

	return true;
}


void NoobEnemy::Update()
{
	int requestState = EnNoobEnemyStateType_Max;
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnNoobEnemyStateType>(requestState);
		m_stateList[m_currentState]->Eneter();
	}
	m_stateList[m_currentState]->Update();

	m_physicsStaticObject.SetPosition(m_position);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}


void NoobEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}