/**
 * @file Game.cpp
 * @brief ゲームメインシーンの実装
 */
#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "Camera/GameCamera.h"
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
#include "Sound/SoundManager.h"
#include "GameClear.h"
#include "GameResult.h" 
#include "DebugCamera.h"

 // 静的メンバ変数の初期化
bool  Game::IsGamePlay = false;
bool  Game::IsPaused = false;
int   Game::DefeatedEnemyCount = 0;
float Game::ClearTime = 0.0f;
float Game::FinalTemperature = 0.0f;
float Game::FinalHP = 0.0f;
int   Game::CoinCount = 0;
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


	// DebugCameraを使って座標を取得する際はコメントアウトしておく
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
	SoundManager::Get().StopBGM();


	StartRefrigeratorSound();


	IsGamePlay = false;
	IsPaused = false;
	// リザルト用変数のリセット
	DefeatedEnemyCount = 0;
	ClearTime = 0.0f;
	FinalTemperature = 4.0f; // GameTimeUIの初期値に合わせる
	FinalHP = 100.0f;

	//// コイン所持数をリセット
	//GetItem::ResetCoinCount();

	CoinCount = 0;

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

	// ゴールを配置する
	GameClear* gameClear = NewGO<GameClear>(0, "gameClear");
	gameClear->SetPosition(Vector3(0.0f, 0.0f, -2850.0f)); // ※ゴールを置きたい座標を指定してください！
	//gameClear->SetPosition(Vector3(100.0f, -200.0f, 0.0f)); // テスト用


	//イベントカメラの生成(デバッグカメラで座標を取得する際はコメントアウトしておく)
	m_eventCamera = NewGO<EventCamera>(0, "eventCamera");


	//============================================================================
	//デバッグカメラの生成(テスト時以外はコメントアウトしておく)
	//============================================================================
	//NewGO<DebugCamera>(0, "debugCamera");
	////　デバッグ中はプレイヤーが動かないように止める
	//if (m_player) {
	//	m_player->SetEnable(false);
	//}
	//============================================================================

	//タイマーリセット
	m_eventStartTimer = 0.0f;
	m_isEventTriggered = false;


	return true;
}


void Game::StartRefrigeratorSound()
{
	// 冷蔵庫の音を再生
	// 第1引数: 音の種類
	// 第2引数: true = ループ再生する (isLood)
	// 第3引数: true = 3Dサウンドにする (is3D)
	m_refrigeratorHandle = SoundManager::Get().PlaySE(enSoundKind_Refrigerator, true, false);
}


/**
 * @brief 毎フレームの更新処理
 * @details
 * ゲームプレイ開始前のロジックとして、カウントダウンの終了を監視します。
 * ゲーム中は各シングルトンマネージャの更新メソッドを呼び出します。
 */
void Game::Update()
{

	// ゲームがまだ始まっていない場合
	if (!IsGamePlay)
	{
		Countdown* countdown = FindGO<Countdown>("countdown");

		// 判定1: カウントダウンが存在し、終了フラグが立っている
		bool isFinished = (countdown != nullptr && countdown->IsFinished());

		// 判定2: カウントダウンが見つからない（＝終了して削除された）
		// ※Startで作ったはずなのにFindGOで取れない場合も「終わった」とみなす安全策です
		bool isGone = (countdown == nullptr);

		if (isFinished || isGone)
		{
			IsGamePlay = true;
			m_countdown = nullptr;

			// ゲーム開始の音
			SoundManager::Get().PlaySE(enSoundKind_RefrigeratorDoor);
			SoundManager::Get().PlayBGM(enSoundKind_Normal);
		}
	}

	// ゲームプレイ中なら時間を進める
	if (IsGamePlay)
	{
		// まだイベントを再生していなければタイマーを進める
		if (!m_isEventTriggered)
		{
			m_eventStartTimer += g_gameTime->GetFrameDeltaTime();

			// 3秒経過したら再生開始
			if (m_eventStartTimer >= 3.0f)
			{
				if (m_eventCamera) {
					m_eventCamera->Play();
					m_isEventTriggered = true;
				}
			}
		}
		// 経過時間を足していく
		ClearTime += g_gameTime->GetFrameDeltaTime();
		// 温度を足していく
		FinalTemperature = 4.0f + (ClearTime / 5.0f) * 0.1f;
	}


	if (m_refrigeratorHandle != INVALID_SOUND_HANDLE)
	{
		// 音源を取得
		SoundSource* se = SoundManager::Get().FindSE(m_refrigeratorHandle);
		if (se != nullptr && m_player != nullptr)
		{

			Vector3 diff = m_player->GetPosition() - m_fridgePos;
			float distance = diff.Length();// 中心からの距離


			float volume = 0.0f;


			//範囲に応じた音量計算
			if (distance <= m_minRange)
			{
				// 最大音量で流す範囲内なら MAX
				volume = 1.0f;
			}
			else if (distance >= m_maxRange)
			{
				// 減衰範囲の外なら 無音
				volume = 0.0f;
			}
			else
			{
				// 減衰させていく範囲（割合計算）
				// (現在の距離 - 最大音量距離) / (減衰区間の長さ) で 0.0～1.0 の割合を出す
				float rate = (distance - m_minRange) / (m_maxRange - m_minRange);
				//距離が離れるほど音量を下げる (1.0 -> 0.0)
				volume = 1.0f - rate;
			}
			// 計算した音量を適用
			se->SetVolume(volume);
		}
	}

	// ---------------------------------------------------------
	// ボス出現イベントの監視
	// ---------------------------------------------------------
	if (StageManager::GetInstance()->IsBossEventRequested())
	{
		// 合図を受け取ったので、フラグを下ろす
		StageManager::GetInstance()->ClearBossEventRequest();

		// ノーマルBGMを停止する
		SoundManager::Get().StopBGM();

		// イベントカメラがあるか確認
		if (m_eventCamera) {
			// ボス用のルートデータをロード
			m_eventCamera->LoadBossPath();

			// 再生開始！
			m_eventCamera->Play();

			// BGMを変えたりするならここ
		}
	}

	// ---------------------------------------------------------
	// カメラの更新制御
	// ---------------------------------------------------------


	// イベント再生中かどうか判定
	bool isEventPlaying = (m_eventCamera && m_eventCamera->IsPlaying());

	if (isEventPlaying)
	{
		// イベント中：GameCameraを無効化（止める）
		if (m_gameCamera) {
			m_gameCamera->SetEnable(false);
		}

		// m_eventCamera->Update() は書きません！
		// NewGOで作ったオブジェクトはエンジンが自動でUpdateを呼んでくれるため、
		// ここで書くと2回実行されてしまいます。
	}
	else
	{
		// イベント外：GameCameraを有効化（動かす）
		if (m_gameCamera) {
			m_gameCamera->SetEnable(true);
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

	SoundManager::Get().Update();
}


void Game::Render(RenderContext& rc)
{
}