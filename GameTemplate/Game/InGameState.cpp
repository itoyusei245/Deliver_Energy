/**
 * @file InGameState.cpp
 * @brief ゲーム本編ステートの実装
 */
#include "stdafx.h"
#include "InGameState.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "BackGround.h"
#include "Enemy/EnemyManager.h"
#include "StageManager.h"
#include "Countdown.h"
#include "GameTimeUI.h"
#include "CoinUI.h"
#include "Pause.h"

 // 静的変数の実体定義
bool InGameState::IsGamePlay = false;
bool InGameState::IsPaused = false;
int InGameState::CoinCount = 0;

InGameState::InGameState()
{
}

InGameState::~InGameState()
{
    // デストラクタでオブジェクト削除を呼ぶ（安全策）
    DeleteGameObjects();
}

/**
 * @brief シーン開始時の初期化処理
 * @details
 * 1. StageManager, EnemyManagerのシングルトン生成とセットアップ
 * 2. ゲーム進行フラグのリセット
 * 3. プレイヤー、カメラ、背景、UI、ポーズ機能の生成
 */
void InGameState::OnEnter()
{
    // === 1. シングルトン初期化 ===
    StageManager::CreateInstance();
    StageManager::GetInstance()->Setup();
    EnemyManager::CreateInstance();
    EnemyManager::GetInstance()->Setup();

    // === 2. 静的変数初期化 ===
    IsGamePlay = false;
    IsPaused = false;
    CoinCount = 0;

    // === 3. オブジェクト生成とポインタ保持 ===
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
    m_backGround = NewGO<BackGround>(0, "background");

    m_countdown = NewGO<Countdown>(0, "countdown");
    m_gameTimeUI = NewGO<GameTimeUI>(0, "gameTimeUI");
    m_coinUI = NewGO<CoinUI>(0, "coinUI");
    m_pause = NewGO<Pause>(0, "pauseFilter");
}

/**
 * @brief 毎フレームの更新処理
 * @details カウントダウン終了を監視してゲームプレイフラグを立てます。また、各マネージャを更新します。
 */
void InGameState::OnUpdate()
{
    // カウントダウン終了判定
    if (!IsGamePlay && m_countdown && m_countdown->IsFinished())
    {
        IsGamePlay = true;
    }

    // マネージャーの更新 
    StageManager::GetInstance()->Update();
    EnemyManager::GetInstance()->Update();
}

void InGameState::OnExit()
{
}

bool InGameState::ShouldChangeState()
{
    return false;
}

/**
 * @brief シーン内の全オブジェクトを削除する
 * @details 保持しているポインタを使ってDeleteGOを行い、ポインタをnullptrで無効化します。
 */
void InGameState::DeleteGameObjects()
{
    DeleteGO(m_player);
    DeleteGO(m_gameCamera);
    DeleteGO(m_backGround);
    DeleteGO(m_countdown);
    DeleteGO(m_gameTimeUI);
    DeleteGO(m_coinUI);
    DeleteGO(m_pause);

    m_player = nullptr;
    m_gameCamera = nullptr;
    m_backGround = nullptr;
    m_countdown = nullptr;
    m_gameTimeUI = nullptr;
    m_coinUI = nullptr;
    m_pause = nullptr;
}

void InGameState::Render(RenderContext& rc)
{
}