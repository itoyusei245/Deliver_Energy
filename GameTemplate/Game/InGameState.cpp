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
#include "UI/GameTimeUI.h"
#include "UI/CoinUI.h"
#include "Pause.h"

namespace {
    constexpr int PRIO_DEFAULT = 0;

    constexpr const char* NAME_PLAYER = "player";
    constexpr const char* NAME_CAMERA = "gameCamera";
    constexpr const char* NAME_BACKGROUND = "background";
    constexpr const char* NAME_COUNTDOWN = "countdown";
    constexpr const char* NAME_TIME_UI = "gameTimeUI";
    constexpr const char* NAME_COIN_UI = "coinUI";
    constexpr const char* NAME_PAUSE = "pauseFilter";
}

// 静的変数の実体定義
bool InGameState::IsGamePlay = false;
bool InGameState::IsPaused = false;
int InGameState::CoinCount = 0;

InGameState::InGameState()
{
}

InGameState::~InGameState()
{
    // デストラクタでオブジェクト削除を呼ぶ
    DeleteGameObjects();
}

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
    // 定数を使って安全に生成！
    m_player = NewGO<Player>(PRIO_DEFAULT, NAME_PLAYER);
    m_gameCamera = NewGO<GameCamera>(PRIO_DEFAULT, NAME_CAMERA);
    m_backGround = NewGO<BackGround>(PRIO_DEFAULT, NAME_BACKGROUND);

    m_countdown = NewGO<Countdown>(PRIO_DEFAULT, NAME_COUNTDOWN);
    m_gameTimeUI = NewGO<GameTimeUI>(PRIO_DEFAULT, NAME_TIME_UI);
    m_coinUI = NewGO<CoinUI>(PRIO_DEFAULT, NAME_COIN_UI);
    m_pause = NewGO<Pause>(PRIO_DEFAULT, NAME_PAUSE);
}

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