#pragma once
class Game;
class Image :public IGameObject
{
public:
	Image();
	~Image();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);
	Game* game;
	//�����o�ϐ�
	SpriteRender spriteRender; //�X�v���C�g�����_�\
};