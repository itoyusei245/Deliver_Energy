#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
#include"GameCamera.h"
#include"Block.h"
//#include"Block02.h"
//#include"Block03.h"
//#include"Block04.h"
#include"Item.h"
#include"Image.h"
#include"Image02.h"
#include"Image03.h"
#include"Image04.h"
#include"GameClear.h"
Game::Game()
{
	//�v���C���[�̃I�u�W�F�N�g�����
	m_player = NewGO<Player>(0, "player");
	//�Q�[���J�����̃I�u�W�F�N�g�����
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//�w�i�̃I�u�W�F�N�g�����
	m_backGround = NewGO<BackGround>(0);

	m_block = NewGO<Block>(0);
	

	Item* item2 = NewGO<Item>(0, "item");
	item2->position = { -10.0f,550.0f,1580.0f };
	item2->firstPosition = item2->position;

	Item* item3 = NewGO<Item>(20, "item");
	item3->position = { -10.0f,1050.0f,3220.0f };
	item3->firstPosition = item3->position;

	Item* item4 = NewGO<Item>(0, "item");
	item4->position = { -10.0f,340.0f,4650.0f };
	item4->firstPosition = item4->position;

	Item* item5 = NewGO<Item>(0, "item");
	item5->position = { -10.0f,100.0f,7410.0f };
	item5->firstPosition = item5->position;
}

Game::~Game()
{
	DeleteGO(m_block);
	//�w�i���폜
	DeleteGO(m_backGround);
	//�Q�[���J�������폜
	DeleteGO(m_gameCamera);
	//�v���C���[���폜
	DeleteGO(m_player);
}

bool Game::Start()
{
	//Init=������
	NewGO<BackGround>(0, "background");
	m_block = NewGO<Block>(0,"block");//�u���b�N�̃C���X�^���X��m_block�Ɋi�[�A�����NewGO�����I�u�W�F�N�g�̏����擾�ł���悤�ɂȂ�B
	return true;
}

void Game::Update()
{
	//�v���C���[����Block�Ɍ������x�N�g�����v�Z
	Vector3 diff =  m_player->position - m_block->position;
	float Block = diff.Length();
	//�x�N�g���̒�����120.0f��菬����������
	if (Block <= 80.0f && m_block->m_isGOKoin)
	{
		//���g���o��������B
		Item* item1 = NewGO<Item>(0, "item");
		item1->position = { -10.0f,300.0f,0.0f };
		item1->firstPosition = item1->position;
		m_block->m_isGOKoin = false;
	}

	if (m_player->itemCount == 1)
	{
		if (m_image ==true)
		{
			NewGO<Image>(0, "image");
			m_image = false;
		}

	}

	if (m_player->itemCount == 2)
	{
		if (m_image02 == true)
		{
			NewGO<Image02>(0, "image02");
			m_image02 = false;
		}
	}

	if (m_player->itemCount == 3)
	{
		if (m_image03 == true)
		{
			NewGO<Image03>(0, "image03");
			m_image03 = false;
		}
	}

	if (m_player->itemCount == 4)
	{
		if (m_image04 == true)
		{
			NewGO<Image04>(0, "image04");
			m_image04 = false;
		}
	}

	if (m_player->itemCount == 5)
	{
		NewGO<GameClear>(0, "gameClear");
		//���g���폜����B
		DeleteGO(this);
	}
}

void Game::Render(RenderContext& rc)
{

}