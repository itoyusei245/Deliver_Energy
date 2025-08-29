#include "stdafx.h"
#include "GameClear.h"
#include"Title.h"

GameClear::GameClear()
{
	//�Q�[���N���A�̉摜��ǂݍ���
	spriteRender.Init("Assets/sprite/Clear.dds", 1920.0f, 1080.0f);
}

GameClear::~GameClear()
{

}

//�X�V����
void GameClear::Update()
{
	//A�{�^���������ꂽ��c
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�^�C�g���̃I�u�W�F�N�g�����I
		NewGO<Title>(0, "title");
		//���g���폜����
		DeleteGO(this);
	}
}

//�`�揈��
void GameClear::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}