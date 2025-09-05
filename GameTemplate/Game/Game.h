#pragma once
#include "Level3DRender/LevelRender.h"
class Player;
class Enemy;
class GameCamera;
class BackGround;
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
	ModelRender	 m_modelRender;
	Vector3		 m_pos = Vector3::Zero;
	Player*		 m_player = nullptr;
	Enemy*		 m_enemy = nullptr;
	GameCamera*	 m_gameCamera;
	BackGround*	 m_backGround;
	Vector3		 position;
	SpriteRender spriteRender;
};