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

class InGameState : public GameStateBase
{
public:
	InGameState();
	~InGameState();

	void OnUpdate() override;
	void OnEnter() override;
	void OnExit() override;
	bool ShouldChangeState() override;
	void Render(RenderContext& rc) override;

	// 管理している全ゲームオブジェクトを削除する
	void DeleteGameObjects();

	// --- 静的ステート変数 ---
	static bool IsGamePlay; // ゲームがプレイ可能状態か（カウントダウン終了後true）
	static bool IsPaused;   // ポーズ中かどうか
	static int  CoinCount;   // 現在のコイン獲得数

private:
	Player*		m_player	 = nullptr;
	GameCamera* m_gameCamera = nullptr;
	BackGround* m_backGround = nullptr;
	Countdown*  m_countdown  = nullptr;
	GameTimeUI* m_gameTimeUI = nullptr;
	CoinUI*		m_coinUI	 = nullptr;
	Pause*		m_pause		 = nullptr;
};