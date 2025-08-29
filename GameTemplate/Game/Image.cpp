#include "stdafx.h"
#include "Image.h"
#include "Game.h"
Image::Image()
{
	//‰æ‘œ‚ğ“Ç‚İ‚İ
	spriteRender.Init("Assets/Data/Data03.dds", 1920.0f, 1080.0f);
	//spriteRender[1].Init("Assets/Data/Data02.dds", 1920.0f, 1080.0f);
	//spriteRender[2].Init("Assets/Data/Data03.dds", 1920.0f, 1080.0f);
	//spriteRender[3].Init("Assets/Data/Data04.dds", 1920.0f, 1080.0f);
	//game = FindGO < Game>("game");
}

Image::~Image()
{

}

void Image::Update()
{
	

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//game->imageState++;
		//©g‚ğíœ‚·‚é
		DeleteGO(this);
	}

}

//•`‰æˆ—
void Image::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}