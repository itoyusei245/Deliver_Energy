#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
#include"Enemy.h"
#include"GameCamera.h"
Game::Game()
{
	//プレイヤーのオブジェクトを作る
	m_player = NewGO<Player>(0, "player");
	//敵のオブジェクトを作る
	m_enemy = NewGO<Enemy>(0, "enemy");
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
	//敵を削除
	DeleteGO(m_enemy);
	//プレイヤーを削除
	DeleteGO(m_player);
}

bool Game::Start()
{
	//Init=初期化
	NewGO<BackGround>(0, "background");
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();//ワイヤーフレームを描画する
	return true;
}

void Game::Update()
{
	
}

void Game::Render(RenderContext& rc)
{

}