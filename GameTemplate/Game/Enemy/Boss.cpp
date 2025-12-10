#include "stdafx.h"
#include "Boss.h"


Boss::Boss()
{
	m_status = new BossStatus();
	m_status->Setup();
}


Boss::~Boss()
{
	delete m_status;
	m_status = nullptr;

	for (auto* state : m_stateList) {
		delete state;
	}
}


bool Boss::Start()
{
	SetScale(Vector3(65.0f, 65.0f, 65.0f));

	m_stateList[enBossStateType_Idle] = new BossIdleState(this);
	m_stateList[enBossStateType_Move] = new BossMoveState(this);
	m_stateList[enBossStateType_CreateFamiliar] = new BossCreateFamiliarState(this);
	m_modelRender.Init("Assets/animData/main_bossEnemy.tkm");
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}


void Boss::Update()
{
	int requestState = EnBossStateType_Max;
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnBossStateType>(requestState);
		m_stateList[m_currentState]->Eneter();
	}
	m_stateList[m_currentState]->Update();

	m_physicsStaticObject.SetPosition(m_position);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}


void Boss::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}