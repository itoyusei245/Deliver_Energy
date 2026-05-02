/**
 * @file GameTimeUI.cpp
 * @brief ゲームタイム（温度）UIの実装
 */
#include "stdafx.h"
#include "GameTimeUI.h"
#include "Game.h"
#include "Camera/EventCamera.h"

 // --- マジックナンバーを排除するための定数 ---
namespace {
    const char* const NUM_DIR     = "Assets/sprite/";
    const char* const NUM_EXT     = ".DDS";
    const float       NUM_SIZE    = 50.0f;
    const float       NUM_SPACING = 50.0f;

    const char* const SEP_TEX = "Assets/sprite/dot.DDS";
    const char* const SUF_TEX = "Assets/sprite/timer.DDS";
    const float       BASE_Y  = 450.0f;

    // 各UIの配置X座標
    const float       POS_X_TENS  = 760.0f; // 整数が2桁(10以上)の時の開始位置
    const float       POS_X_ONES  = 810.0f; // 整数が1桁(9以下)の時の開始位置
    const float       POS_X_SEP   = 840.0f; // 小数点
    const float       POS_X_RIGHT = 870.0f; // 小数部
    const float       POS_X_SUF   = 920.0f; // 単位
}

GameTimeUI::GameTimeUI()
{
    m_canvas = new UICanvas();

    // 1. 整数部の初期化
    m_leftNumber = m_canvas->CreateUI<UINumber>();
    m_leftNumber->Initialize(NUM_DIR, NUM_EXT, NUM_SIZE, NUM_SIZE, Vector3(POS_X_ONES, BASE_Y, 0.0f), NUM_SPACING);
    m_leftNumber->SetZeroPadding(false);

    // 2. 小数点（ドット）の初期化
    m_separator = m_canvas->CreateUI<UIIcon>();
    m_separator->Initialize(SEP_TEX, NUM_SIZE, NUM_SIZE, Vector3(POS_X_SEP, BASE_Y, 0.0f), Vector3::One, Quaternion::Identity);

    // 3. 小数部の初期化
    m_rightNumber = m_canvas->CreateUI<UINumber>();
    m_rightNumber->Initialize(NUM_DIR, NUM_EXT, NUM_SIZE, NUM_SIZE, Vector3(POS_X_RIGHT, BASE_Y, 0.0f), NUM_SPACING);
    m_rightNumber->SetZeroPadding(false);

    // 4. 単位アイコンの初期化
    m_suffix = m_canvas->CreateUI<UIIcon>();
    m_suffix->Initialize(SUF_TEX, NUM_SIZE, NUM_SIZE, Vector3(POS_X_SUF, BASE_Y, 0.0f), Vector3::One, Quaternion::Identity);

    m_timer = 0.0f;
}

GameTimeUI::~GameTimeUI()
{
    delete m_canvas;
}

void GameTimeUI::Update()
{
    if (!Game::IsGamePlay) return;
    if (Game::IsPaused) return;

    m_timer += g_gameTime->GetFrameDeltaTime();

    // --- 計算ロジック ---
    int totalCounts = (int)(m_timer / 5.0f);
    int valRight = totalCounts % 10;
    int valLeftTotal = 4 + (totalCounts / 10);

    // --- UINumber への反映 ---
    // 桁数による座標の調整
    if (valLeftTotal >= 10) {
        m_leftNumber->m_transform.m_localPosition.x = POS_X_TENS;
    }
    else {
        m_leftNumber->m_transform.m_localPosition.x = POS_X_ONES;
    }

    m_leftNumber->SetNumber(valLeftTotal);
    m_rightNumber->SetNumber(valRight);

    // キャンバス全体を更新
    m_canvas->Update();
}

void GameTimeUI::Render(RenderContext& rc)
{
    if (!Game::IsGamePlay) return;
    if (EventCamera::IsEventPlaying) return;

    m_canvas->Render(rc);
}