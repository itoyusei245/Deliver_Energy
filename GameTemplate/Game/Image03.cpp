#include "stdafx.h"
#include "Image03.h"
Image03::Image03()
{
	//�摜��ǂݍ���
	//spriteRender.Init("Assets/Data/Data01.dds", 1920.0f, 1080.0f);
	//spriteRender[1].Init("Assets/Data/Data02.dds", 1920.0f, 1080.0f);
	spriteRender.Init("Assets/Data/Data01.dds", 1920.0f, 1080.0f);
	//spriteRender.Init("Assets/Data/Data04.dds", 1920.0f, 1080.0f);
	//game = FindGO < Game>("game");
}

Image03::~Image03()
{

}

void Image03::Update()
{


	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//game->imageState++;
		//���g���폜����
		DeleteGO(this);
	}

}

//�`�揈��
void Image03::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}