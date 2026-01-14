#pragma once
class AthleticTrigger:public IGameObject
{
public:
	AthleticTrigger();
	~AthleticTrigger();


	bool Start()override;
	void Update()override{}

private:
	LevelRender m_levelRender;
};


