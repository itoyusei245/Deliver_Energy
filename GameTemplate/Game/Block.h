#pragma once
class Block:public IGameObject
{
public:
	Block();
	~Block();

	void Render(RenderContext& rc);

	ModelRender modelRender;
	Quaternion m_rot;
	PhysicsStaticObject physicsStaticObject;
	Vector3 position;
	bool m_isGOKoin = true;

};

