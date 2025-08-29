#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
#include"GameCamera.h"
Game::Game()
{
	//プレイヤーのオブジェクトを作る
	m_player = NewGO<Player>(0, "player");
	//ゲームカメラのオブジェクトを作る
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	//背景のオブジェクトを作る
	m_backGround = NewGO<BackGround>(0);
}

Game::~Game()
{
	//背景を削除
	DeleteGO(m_backGround);
	//ゲームカメラを削除
	DeleteGO(m_gameCamera);
	//プレイヤーを削除
	DeleteGO(m_player);
}

bool Game::Start()
{
	//Init=初期化
	NewGO<BackGround>(0, "background");
	//m_block = NewGO<Block>(0,"block");//ブロックのインスタンスをm_blockに格納、これでNewGOしたオブジェクトの情報を取得できるようになる。
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}