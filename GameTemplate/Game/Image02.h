#pragma once
class Game;
class Image02 :public IGameObject
{
public:
	Image02();
	~Image02();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);
	Game* game;
	//�����o�ϐ�
	SpriteRender spriteRender; //�X�v���C�g�����_�\
};
