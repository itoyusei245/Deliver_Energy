/**
 * @file Game.cpp
 * @brief ゲームメインシーンの実装
 */
#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "Athletic/AthleticManager.h"
#include "StageManager.h"
#include "Countdown.h"
#include "GameTimeUI.h"
#include "GetItem.h"
#include "CoinUI.h"
#include "GameUI.h"
#include "Pause.h"
#include "Collision/CollisionHitManager.h"

 // 静的メンバ変数の初期化
bool Game::IsGamePlay = false;
bool Game::IsPaused = false;

/**
 * @brief コンストラクタ
 * @details
 * ゲームループのかなり早い段階で必要なオブジェクト（Player, Camera）を生成します。
 * 背景(BackGround)の生成はStartで行うため、ここではコメントアウトされています。
 */
Game::Game()
{
	/** プレイヤーのオブジェクトを作成 */
	m_player = NewGO<Player>(0, "player");

	/** ゲームカメラのオブジェクトを作成し、プレイヤーの参照を渡す */
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_gameCamera->player = m_player;

	/** 背景のオブジェクトを作成（※Startへ移動済み） */
	//m_backGround = NewGO<BackGround>(0);
}


/**
 * @brief デストラクタ
 * @details
 * メモリリークを防ぐため、保持しているGameObjectを削除(DeleteGO)し、
 * 各種シングルトンマネージャのインスタンスを破棄(DeleteInstance)します。
 */
Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_gameCamera);
	/** プレイヤーを削除*/
	DeleteGO(m_player);
	DeleteGO(m_gameUI);

	/** アスレチック管理クラスのインスタンスを削除*/
	AthleticManager::DeleteInstance();

	/** 敵管理クラスのインスタンスを削除*/
	EnemyManager::DeleteInstance();

	/** ステージ管理クラスのインスタンスを削除*/
	StageManager::DeleteInstance();

	DeleteGO(m_collisionHitManagerObject);
}


/**
 * @brief ゲーム開始時の初期化処理
 * @details
 * 1. フラグのリセット（Play=false, Pause=false）
 * 2. コインカウントのリセット
 * 3. UI（コイン、時間、ポーズ、カウントダウン）の生成
 * 4. 当たり判定管理オブジェクトの生成
 * 5. 各種マネージャ（Stage, Enemy, Athletic）のシングルトン生成とSetup実行
 * 6. 背景オブジェクトの生成
 */
bool Game::Start()
{
	IsGamePlay = false;
	IsPaused = false;

	// コイン所持数をリセット
	GetItem::ResetCoinCount();

	// GameUIを生成して、Playerの情報を渡す
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gameUI->SetPlayer(m_player); // これでHPが連携されます

	// --- UI生成 ---
	m_coinUI = NewGO<CoinUI>(0, "coinUI");
	m_gameTimeUI = NewGO<GameTimeUI>(0, "gameTimeUI");
	NewGO<Pause>(0, "pause"); // ポーズ機能は常駐


	// 当たり判定マネージャのラッパーオブジェクト生成
	m_collisionHitManagerObject = NewGO<CollisionHitManagerObject>(0, "collisionHitManagerObject");

	// カウントダウン演出生成
	m_countdown = NewGO<Countdown>(0, "countdown");

	// --- マネージャ群の初期化 ---

	/** ステージ管理クラスのインスタンスを生成・初期化 */
	StageManager::CreateInstance();
	StageManager::GetInstance()->Setup();

	/** 敵管理クラスのインスタンスを生成・初期化 */
	EnemyManager::CreateInstance();
	EnemyManager::GetInstance()->Setup();

	/** アスレチック管理クラスのインスタンスを生成・初期化 */
	AthleticManager::CreateInstance();
	AthleticManager::GetInstance()->Setup();

	/** 背景の初期化（二重生成防止チェック付き） */
	if (m_backGround == nullptr) {
		m_backGround = NewGO<BackGround>(0, "background");
	}
	return true;
}


/**
 * @brief 毎フレームの更新処理
 * @details
 * ゲームプレイ開始前のロジックとして、カウントダウンの終了を監視します。
 * ゲーム中は各シングルトンマネージャの更新メソッドを呼び出します。
 */
void Game::Update()
{
	// ゲームプレイ開始前（カウントダウン中など）の処理
	if (!IsGamePlay)
	{
		/** カウントダウンが終わったらゲーム開始フラグを立てる */
		if (m_countdown && m_countdown->IsFinished())
		{
			IsGamePlay = true;
			m_countdown = nullptr; // DeleteGO済みなのでポインタ切っておく
		}
	}

	// --- 各マネージャの更新 ---

	/**
	 * StageManagerの更新
	 * @note IGameObject継承かどうかに関わらず、ここで明示的に呼び出しているようです。
	 */
	StageManager::GetInstance()->Update();

	/** * EnemyManagerの更新
	 * @note 敵の発生やボス戦の進行を管理します。
	 */
	EnemyManager::GetInstance()->Update();
}


void Game::Render(RenderContext& rc)
{
}