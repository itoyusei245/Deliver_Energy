#include "stdafx.h"
#include "AthleticStage.h"

AthleticStage::AthleticStage()
{
	m_mapAthletic.SetPosition(Vector3(800.0f, -200.0f, -3400.0f));
	m_mapAthletic.Init("Assets/modelData/Stage/map_Athletic.tkm");
	m_mapAthletic.Update();

	physicsStaticObject.CreateFromModel(m_mapAthletic.GetModel(), m_mapAthletic.GetModel().GetWorldMatrix());
}

AthleticStage::~AthleticStage()
{
}

void AthleticStage::Render(RenderContext& rc)
{
	m_mapAthletic.Draw(rc);
}