#pragma once

//�^�C�g���B
class Title : public IGameObject
{
public:
	Title();
	~Title();
	//�X�V����
	void Update();
	//�`��֐�
	void Render(RenderContext& rc);

	SpriteRender spriteRender;
};
