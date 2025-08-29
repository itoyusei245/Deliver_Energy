#pragma once
#include "Level3DRender/LevelRender.h"
class Player;
class GameCamera;
class BackGround;
//Game�V�[�����Ǘ�����N���X
class Game : public IGameObject
{
public:
	Game();
	~Game();
	//�X�V����
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	Vector3		m_pos = Vector3::Zero;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera;
	BackGround* m_backGround;
	Vector3 position;
	SpriteRender spriteRender;
};