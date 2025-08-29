#pragma once
#include "Level3DRender/LevelRender.h"
#include "sound/SoundSource.h"
//Gameシーンを管理するクラス
class Game : public IGameObject
{
public:
	Game();
	~Game();
	//更新処理
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
};