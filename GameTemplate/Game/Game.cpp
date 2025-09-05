#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
#include"Enemy.h"
#include"GameCamera.h"
Game::Game()
{
	//�v���C���[�̃I�u�W�F�N�g�����
	m_player = NewGO<Player>(0, "player");
	//�G�̃I�u�W�F�N�g�����
	m_enemy = NewGO<Enemy>(0, "enemy");
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
	//�G���폜
	DeleteGO(m_enemy);
	//�v���C���[���폜
	DeleteGO(m_player);
}

bool Game::Start()
{
	//Init=������
	NewGO<BackGround>(0, "background");
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();//���C���[�t���[����`�悷��
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}