#include "stdafx.h"
#include "Block.h"
#include"BackGround.h"

Block::Block()
{
	modelRender.Init("Assets/modelData/map/BLOCK.tkm");
	position = { 10.0f, 190.0f, 0.0f };
	modelRender.SetPosition(position);
	m_rot.SetRotationDegY(90);
	modelRender.SetRotation(m_rot);
	modelRender.SetScale(Vector3(2.5f, 2.5f, 2.5f));
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

Block::~Block()
{

}

//void Block::Update()
//{
//
//}

void Block::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}