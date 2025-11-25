#include "stdafx.h"
#include "Game.h"
#include"BackGround.h"
#include"Player.h"
//#include"BossEnemy.h"
#include"GameCamera.h"
#include"EnemyManager.h"
#include"StageManager.h"
#include "Countdown.h"
#include "GameTimeUI.h"

/**
 * @brief Gameクラスのコンストラクタ
 * @details プレイヤー、敵、ゲームカメラ、背景の各オブジェクトを生成します。
 */
bool Game::IsGamePlay = false;


Game::Game()
{
	/** プレイヤーのオブジェクトを作成: */
	m_player = NewGO<Player>(0, "player");
	/** ゲームカメラのオブジェクトを作成*/
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	/** 背景のオブジェクトを作成*/
	m_backGround = NewGO<BackGround>(0);
}

/**
 * @brief Gameクラスのデストラクタ
 * @details 生成した背景、ゲームカメラ、敵、プレイヤーの各オブジェクトを削除します。
 */
Game::~Game()
{
	/** 背景を削除*/
	DeleteGO(m_backGround);
	/** ゲームカメラを削除*/
	DeleteGO(m_gameCamera);
	/** プレイヤーを削除*/
	DeleteGO(m_player);
	/** 敵管理クラスのインスタンスを削除*/
	EnemyManager::DeleteInstance();
	/**ステージクラスのインスタンスを削除*/
	StageManager::DeleteInstance();
}

/**
 * @brief ゲーム開始時の初期化処理
 * @return 初期化が成功した場合はtrue
 */
bool Game::Start()
{
	IsGamePlay = false;
	m_gameTimeUI = NewGO<GameTimeUI>(0, "gameTimeUI");
	m_countdown = NewGO<Countdown>(0, "countdown");
	/**ステージ管理クラスのインスタンスを生成*/
	StageManager::CreateInstance();
	/** ステージ管理クラスのインスタンスを初期化*/
	StageManager::GetInstance()->Setup();
	/** 敵管理クラスのインスタンスを生成*/
	EnemyManager::CreateInstance();
	/** 敵管理クラスのインスタンスを初期化*/
	EnemyManager::GetInstance()->Setup();
	/** 背景の初期化*/
	NewGO<BackGround>(0, "background");

	return true;
}

/**
 * @brief 毎フレームの更新処理
 */
void Game::Update()
{
	if (!IsGamePlay)
	{
		// カウントダウンが終わったらゲーム開始フラグを立てる
		if (m_countdown && m_countdown->IsFinished())
		{
			IsGamePlay = true;
			m_countdown = nullptr; // DeleteGO済みなのでポインタ切っておく
		}
	}
	/**↓IGameOdjectを継承するやりかた↓*/
	StageManager::GetInstance()->Update();
	/**↓IGameOdjectを継承しないやりかた↓*/
	EnemyManager::GetInstance()->Update();
}

void Game::Render(RenderContext& rc)
{

}