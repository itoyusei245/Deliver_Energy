#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
#include"GameCamera.h"
Game::Game()
{
	//�v���C���[�̃I�u�W�F�N�g�����
	m_player = NewGO<Player>(0, "player");
	//�Q�[���J�����̃I�u�W�F�N�g�����
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//�w�i�̃I�u�W�F�N�g�����
	m_backGround = NewGO<BackGround>(0);
}

Game::~Game()
{
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
	//m_block = NewGO<Block>(0,"block");//�u���b�N�̃C���X�^���X��m_block�Ɋi�[�A�����NewGO�����I�u�W�F�N�g�̏����擾�ł���悤�ɂȂ�B
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}