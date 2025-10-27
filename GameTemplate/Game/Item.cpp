#include "stdafx.h"
#include "Item.h"

Item::Item()
{
}

Item::~Item()
{
}

void Item::Init(const Vector3& pos)
{
    m_position = pos;
    m_coin.Init("Assets/modelData/Item/coin.tkm");
    m_coin.SetPosition(m_position);
}

void Item::Update()
{
    /**ƒRƒCƒ“‚Ì‰ñ“]*/
    m_rotation += 1.0f;
    Quaternion rot;
    rot.SetRotationDegY(m_rotation);
    m_coin.SetRotation(rot);


    m_coin.Update();
}

void Item::Render(RenderContext& rc)
{
    m_coin.Draw(rc);
}