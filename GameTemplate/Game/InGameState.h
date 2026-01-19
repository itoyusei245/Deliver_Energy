/**
 * @file InGameState.h
 * @brief ゲーム本編（プレイ中）のステートクラス定義
 */
#pragma once
#include "GameStateBase.h"

 // 前方宣言
class Player;
class GameCamera;
class BackGround;
class EnemyManager;
class StageManager;
class Countdown;
class GameTimeUI;
class CoinUI;
class Pause;

/**
 * @class InGameState
 * @brief ゲームプレイ中の状態を管理するクラス
 * @details
 * GameStateBaseを継承し、ゲーム本編の初期化(OnEnter)、更新(OnUpdate)、終了処理(OnExit)を行います。
 * プレイヤー、カメラ、各種マネージャー、UIの生成と生存期間を管理します。
 */
class InGameState : public GameStateBase
{
public:
	InGameState();
	~InGameState();

	/**
	 * @brief ステート更新処理
	 * @details カウントダウンの監視や、各マネージャーの更新を実行します。
	 */
	void OnUpdate() override;

	/**
	 * @brief ステート開始時処理
	 * @details 各種オブジェクト（Player, Camera, UI等）とシングルトンマネージャーの生成・初期化を行います。
	 */
	void OnEnter() override;

	/**
	 * @brief ステート終了時処理
	 */
	void OnExit() override;

	/**
	 * @brief 遷移判定
	 * @return bool 現状は常にfalse（遷移なし）を返します。
	 */
	bool ShouldChangeState() override;

	/**
	 * @brief 描画処理
	 * @note 各オブジェクトは自己描画するため、ここは空実装です。
	 */
	void Render(RenderContext& rc) override;

	/**
	 * @brief 管理している全ゲームオブジェクトを削除する
	 * @details リトライやタイトルへ戻る際にメモリリークを防ぐため、明示的に呼び出します。
	 */
	void DeleteGameObjects();

	// --- 静的ステート変数 ---
	/** ゲームがプレイ可能状態か（カウントダウン終了後true） */
	static bool IsGamePlay;
	/** ポーズ中かどうか */
	static bool IsPaused;
	/** 現在のコイン獲得数 */
	static int CoinCount;

private:
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
	BackGround* m_backGround = nullptr;
	Countdown* m_countdown = nullptr;
	GameTimeUI* m_gameTimeUI = nullptr;
	CoinUI* m_coinUI = nullptr;
	Pause* m_pause = nullptr;
};