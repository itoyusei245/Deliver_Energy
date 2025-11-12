#pragma once
class AthleticStage:public IGameObject
{
public:
	AthleticStage();
	~AthleticStage();

	void  Render(RenderContext& rc);

	void Init();

	PhysicsStaticObject physicsStaticObject;

private:
	ModelRender m_mapAthletic;
};

