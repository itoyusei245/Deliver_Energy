#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	modelRender.Init("Assets/modelData/Stage/BackGround.tkm");
	modelRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_rot.SetRotationDegY(90);
	modelRender.SetRotation(m_rot);
	modelRender.SetScale(Vector3(1.3f, 1.3f, 1.3f));
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}