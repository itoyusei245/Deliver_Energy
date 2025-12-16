#include "stdafx.h"
#include "Familiar.h"


Familiar::Familiar()
{
	m_status = new FamiliarStatus();
	m_status->Setup();
}


Familiar::~Familiar()
{
	delete m_status;
	m_status = nullptr;

	for (auto* state : m_stateList) {
		delete state;
	}
}


bool Familiar::Start()
{
	SetScale(Vector3(1.7f, 1.7f, 1.7f));

	m_stateList[enFamiliarStateType_Idle] = new FamiliarIdleState(this);
	m_stateList[enFamiliarStateType_Move] = new FamiliarMoveState(this);
	

	switch (m_familiaType)
	{
	case enFamiliarType_A:
		m_modelRender.Init("Assets/animData/bossEnemy_TypeA.tkm");
		break;
	case enFamiliarType_B:
		m_modelRender.Init("Assets/animData/m_boss_Enemy_TypeB.tkm");
		break;
	default:
		break;
	}
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());


	//m_coreModel.Init("Assets/animData/familiarCore.tkm");
	//m_coreModel.SetTRS(Vector3::Zero, m_rotation, m_scale);
	//m_coreModel.Update();
	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	

	m_collisionObject.CreateSphere(m_position, m_rotation, 110.0f);

	return true;
}


void Familiar::Update()
{
	int requestState = EnBossStateType_Max;
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnFamiliarStateType>(requestState);
		m_stateList[m_currentState]->Eneter();
	}
	m_stateList[m_currentState]->Update();

	m_physicsStaticObject.SetPosition(m_position);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	m_collisionObject.SetPosition(m_position);
	m_collisionObject.Update();
}


void Familiar::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


void Familiar::Setup(const enFamiliarType type)
{
	m_familiaType = type;
}