#include "stdafx.h"
#include "InGameState.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Enemy/EnemyManager.h"
#include "StageManager.h"
#include "Countdown.h"
#include "GameTimeUI.h"
#include "CoinUI.h"
#include "Pause.h"

// ★静的変数の実体定義（旧Gameクラスから移動）
bool InGameState::IsGamePlay = false;
bool InGameState::IsPaused = false;
int InGameState::CoinCount = 0;

InGameState::InGameState()
{
    // コンストラクタはシンプルに
}

InGameState::~InGameState()
{
    // デストラクタでオブジェクト削除を呼ぶ
    DeleteGameObjects();
}

// ------------------------------------
// ライフサイクル処理
// ------------------------------------

/**
 * @brief シーン開始時の初期化処理 (旧 Game::Start() の役割)
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

    // === 3. オブジェクト生成とポインタ保持 (旧 Game::Start() の内容) ===
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
    m_backGround = NewGO<BackGround>(0, "background");

    m_countdown = NewGO<Countdown>(0, "countdown");
    m_gameTimeUI = NewGO<GameTimeUI>(0, "gameTimeUI");
    m_coinUI = NewGO<CoinUI>(0, "coinUI");
    m_pause = NewGO<Pause>(0, "pauseFilter");
}

/**
 * @brief 毎フレームの更新処理 (旧 Game::Update() の役割)
 */
void InGameState::OnUpdate()
{
    // カウントダウン終了判定（IsGamePlayフラグの切り替え）
    if (!IsGamePlay && m_countdown && m_countdown->IsFinished())
    {
        IsGamePlay = true;
    }

    // マネージャーの更新 
    StageManager::GetInstance()->Update();
    EnemyManager::GetInstance()->Update();

    // ★遷移要求はShouldChangeState()の結果をGameクラスがチェックします
}

/**
 * @brief シーン終了時のクリーンアップ処理
 */
void InGameState::OnExit()
{
    // BGMの停止など、シーン固有の終了処理があれば記述
}

/**
 * @brief 状態変更の必要性を判定する (Gameクラスがこの戻り値を見て遷移を実行します)
 */
bool InGameState::ShouldChangeState()
{
    // ここにゲームのクリア条件や次のステートへの遷移条件を記述
    return false;
}

// ------------------------------------
// オブジェクト管理
// ------------------------------------

/**
 * @brief シーン内の全オブジェクトを削除する (旧 Gameのデストラクタの役割)
 */
void InGameState::DeleteGameObjects()
{
    // OnEnter()で作成した全オブジェクトをポインタを使って削除
    DeleteGO(m_player);
    DeleteGO(m_gameCamera);
    DeleteGO(m_backGround);
    DeleteGO(m_countdown);
    DeleteGO(m_gameTimeUI);
    DeleteGO(m_coinUI);
    DeleteGO(m_pause);

    // 削除後、ポインタをnullptrにリセット
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
    // 描画処理はそのまま
}