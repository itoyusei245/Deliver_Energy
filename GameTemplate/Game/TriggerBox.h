#pragma once
class TriggerBox:public IGameObject
{
public:
	TriggerBox();

	~TriggerBox();

	void Init();

private:
	LevelRender m_enemySpawnLevel;
	LevelRender m_stageSpawnLevel;
};


