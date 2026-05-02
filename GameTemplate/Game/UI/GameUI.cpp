/**
 * @file GameUI.cpp
 * @brief ゲームUIの実装
 */
#include "stdafx.h"
#include "GameUI.h"
#include "Game.h" 
#include "Camera/EventCamera.h"

 // --- マジックナンバーを排除するための定数 ---
namespace {
    const char* const FRAME_TEX_PATH = "Assets/sprite/Gauge_HP_Flame.DDS";
    const char* const GAUGE_TEX_PATH = "Assets/sprite/Gauge_HP.DDS";
    const float       TEX_SIZE       = 256.0f;
    const Vector3     UI_POS         = { 680.0f, 450.0f, 0.0f };
    const Vector3     UI_SCALE       = { 0.4f, 0.4f, 1.0f };
}

GameUI::GameUI()
{
    // --- UIキャンバスの生成 ---
    m_canvas = new UICanvas();

    // --- フレーム画像の生成と初期化 ---
    m_hpFrame = m_canvas->CreateUI<UIIcon>();
    m_hpFrame->Initialize(
        FRAME_TEX_PATH,
        TEX_SIZE,
        TEX_SIZE,
        UI_POS,
        UI_SCALE,
        Quaternion::Identity
    );

    // --- HPゲージ本体の生成と初期化 ---
    m_hpGauge = m_canvas->CreateUI<UIGauge>();
    m_hpGauge->CircleInitialize(
        GAUGE_TEX_PATH,
        TEX_SIZE,
        TEX_SIZE,
        UI_POS,
        UI_SCALE,
        Quaternion::Identity
    );
}

GameUI::~GameUI()
{
    delete m_canvas;
}

void GameUI::Update()
{
    // ゲームプレイ中(カウントダウン後)でなければ何もしない
    if (!Game::IsGamePlay) return;

    // プレイヤーがセットされていたらHPを取得してゲージに反映
    if (m_player != nullptr)
    {
        float currentRate = m_player->GetHPRate();
        m_hpGauge->SetProgress(currentRate);
    }

    // キャンバスを更新
    m_canvas->Update();
}

void GameUI::Render(RenderContext& rc)
{
    // ゲームプレイ中でなければ描画しない
    if (!Game::IsGamePlay) return;

    // イベント中なら描画せずに帰る
    if (EventCamera::IsEventPlaying) return;

    // キャンバスを描画
    m_canvas->Render(rc);
}