#pragma once
class Game;
class Image04 :public IGameObject
{
public:
	Image04();
	~Image04();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);
	Game* game;
	//�����o�ϐ�
	SpriteRender spriteRender; //�X�v���C�g�����_�\
};

