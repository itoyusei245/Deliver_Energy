#include "stdafx.h"
#include "Image02.h"

Image02::Image02()
{
	//�摜��ǂݍ���
	//spriteRender.Init("Assets/Data/Data01.dds", 1920.0f, 1080.0f);
	spriteRender.Init("Assets/Data/Data02.dds", 1920.0f, 1080.0f);
	//spriteRender[2].Init("Assets/Data/Data03.dds", 1920.0f, 1080.0f);
	//spriteRender[3].Init("Assets/Data/Data04.dds", 1920.0f, 1080.0f);
	//game = FindGO < Game>("game");
}

Image02::~Image02()
{

}

void Image02::Update()
{


	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//game->imageState++;
		//���g���폜����
		DeleteGO(this);
	}

}

//�`�揈��
void Image02::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}
