#pragma once
#include "Level3DRender/LevelRender.h"

//Gameシーンを管理するクラス
class Game : public IGameObject
{
public:
	Game();
	~Game();
	//更新処理
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
};