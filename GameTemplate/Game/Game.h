#pragma once
#include "Level3DRender/LevelRender.h"
#include "sound/SoundSource.h"
class Player;
class GameCamera;
class BackGround;
class Block;
//class Block02;
//class Block03;
//class Block04;

//class Item;

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

	int imageState = 0;
private:
	bool m_image = true;
	bool m_image02 = true;
	bool m_image03 = true;
	bool m_image04 = true;
	ModelRender m_modelRender;
    Vector3		m_pos = Vector3::Zero;
	Player*		m_player = nullptr;
	GameCamera* m_gameCamera;
	BackGround* m_backGround;
	//Item* item;
	Block* m_block;
	/*Block02* m_block02;
	Block03* m_block03;
	Block04* m_block04;*/
	
	Vector3 position;
	SpriteRender spriteRender;
	SoundSource* gameBGM;		//ゲーム中のBGM。
};