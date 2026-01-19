#include "stdafx.h"
#include "GameUI.h"
#include "Game.h" 
/**
 * @brief コンストラクタ
 * @details ここで画像（HPゲージ）の初期化を行います。Titleクラスと同じタイミングです。
 */
GameUI::GameUI()
{
    // --- HPゲージの初期化 ---
    // 画像パス、サイズ、ブレンドモードを指定
    m_hpGauge.CircleInit(
        m_gaugeProgress,    // 変数への参照を渡す
        m_gaugeSize,        // 変数への参照を渡す
        "Assets/sprite/Gauge_HP.DDS",
        256.0f, 256.0f,
        AlphaBlendMode_Trans
    );
    // 位置や大きさの調整
    Vector3 pos = { 680.0f, 450.0f, 0.0f };
    m_hpGauge.SetPosition(pos);
    m_hpGauge.SetScale({ 0.4f, 0.4f, 1.0f });


    // --- フレーム画像の初期化 ---
    m_hpFrame.Init(
        "Assets/sprite/Gauge_HP_Flame.DDS", 
        256.0f, 256.0f,  
        AlphaBlendMode_Trans
    );


    m_hpFrame.SetPosition(pos);
    m_hpFrame.SetScale({ 0.4f, 0.4f, 1.0f });
}

GameUI::~GameUI()
{
}

/**
 * @brief 更新処理
 */
void GameUI::Update()
{
    // ゲームプレイ中(カウントダウン後)でなければ何もしない
    if (!Game::IsGamePlay) return;


    // プレイヤーがセットされていたらHPを取得してゲージに反映
    if (m_player != nullptr)
    {
        // Playerに追加したGetHPRate()を使う想定
        float currentRate = m_player->GetHPRate();


        // セット用の関数(SetCircleProgress)を呼ぶ。
        m_hpGauge.SetCircleProgress(currentRate);
    }

    // スプライトの更新
    m_hpGauge.Update();
    m_hpFrame.Update();
}

/**
 * @brief 描画処理
 */
void GameUI::Render(RenderContext& rc)
{
    // ゲームプレイ中でなければ描画しない
    if (!Game::IsGamePlay) return;


    m_hpFrame.Draw(rc);
    m_hpGauge.Draw(rc);
}