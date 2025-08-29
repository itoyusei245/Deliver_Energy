#pragma once
#include "Level3DRender/LevelRender.h"
#include "sound/SoundSource.h"
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
};