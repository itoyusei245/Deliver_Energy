#pragma once
class Item:public IGameObject
{
public:
	Item();

	~Item();

	void Init(const Vector3& pos);
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_coin;
	Vector3 m_position=Vector3::Zero;
	float m_rotation = 0.0f;
	
};

