/**
 * @file CoinUI.cpp
 * @brief コインUIの実装
 */
#include "stdafx.h"
#include "CoinUI.h"
#include "GetItem.h"
#include "Game.h"
#include "Camera/EventCamera.h"

 // --- マジックナンバーを排除するための定数 ---
namespace {
    const char* const ICON_TEX_PATH = "Assets/sprite/coin.DDS";
    const float       ICON_SIZE     = 65.0f;
    const Vector3     ICON_POS      = { -900.0f, 450.0f, 0.0f };

    const char* const NUM_DIR       = "Assets/sprite/";
    const char* const NUM_EXT       = ".DDS";
    const float       NUM_SIZE      = 50.0f;
    const Vector3     NUM_START_POS = { -850.0f, 450.0f, 0.0f }; // 数字の開始位置
    const float       NUM_SPACING   = 50.0f; // 数字同士の間隔
}

CoinUI::CoinUI()
{
    // --- キャンバスの生成 ---
    m_canvas = new UICanvas();

    // --- コインアイコンの生成 ---
    m_coinIcon = m_canvas->CreateUI<UIIcon>();
    m_coinIcon->Initialize(
        ICON_TEX_PATH,
        ICON_SIZE,
        ICON_SIZE,
        ICON_POS,
        Vector3::One,
        Quaternion::Identity
    );

    // --- 数字UIの生成 ---
    m_coinNumber = m_canvas->CreateUI<UINumber>();
    m_coinNumber->Initialize(
        NUM_DIR,
        NUM_EXT,
        NUM_SIZE,
        NUM_SIZE,
        NUM_START_POS,
        NUM_SPACING
    );
    m_coinNumber->SetZeroPadding(false);
}

CoinUI::~CoinUI()
{
    delete m_canvas;
}

void CoinUI::Update()
{
    // ゲームプレイ中のみ更新
    if (!Game::IsGamePlay) return;

    // コイン数を取得して安全策（99枚上限）を適用
    int count = GetItem::GetCoinCount();
    if (count > 99) count = 99;

    m_coinNumber->SetNumber(count);

    // キャンバス全体を更新
    m_canvas->Update();
}

void CoinUI::Render(RenderContext& rc)
{
    if (!Game::IsGamePlay) return;
    if (EventCamera::IsEventPlaying) return;

    // キャンバス全体を描画
    m_canvas->Render(rc);
}