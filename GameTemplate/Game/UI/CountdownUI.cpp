/**
 * @file CountdownUI.cpp
 * @brief カウントダウンUIの実装
 */
#include "stdafx.h"
#include "CountdownUI.h"

namespace {
    // --- 画像パス ---
    constexpr const char* TEX_OPERATION = "Assets/sprite/countDownOperation.DDS";
    constexpr const char* TEX_READY = "Assets/sprite/countDown_Ready.DDS";
    constexpr const char* TEX_5 = "Assets/sprite/5.DDS";
    constexpr const char* TEX_4 = "Assets/sprite/4.DDS";
    constexpr const char* TEX_3 = "Assets/sprite/3.DDS";
    constexpr const char* TEX_2 = "Assets/sprite/2.DDS";
    constexpr const char* TEX_1 = "Assets/sprite/1.DDS";
    constexpr const char* TEX_GO = "Assets/sprite/countDown_GO.DDS";

    // --- サイズ定数 ---
    const Vector2 SIZE_FULL(1920.0f, 1080.0f);
    const Vector2 SIZE_NUM(100.0f, 100.0f);
    const Vector3 POS_ZERO(0.0f, 0.0f, 0.0f);
}

CountdownUI::CountdownUI()
{
    m_canvas = new UICanvas();

    // 操作説明画像の初期化
    m_operation = m_canvas->CreateUI<UIIcon>();
    m_operation->Initialize(TEX_OPERATION, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

    // カウントダウン画像の器だけ作成
    m_countdown = m_canvas->CreateUI<UIIcon>();

    // 最初はダミーで初期化しておく
    m_countdown->Initialize(TEX_3, 500.0f, 500.0f, POS_ZERO, Vector3::One, Quaternion::Identity);
    m_countdown->isDraw = false;
}

CountdownUI::~CountdownUI()
{
    delete m_canvas;
}

void CountdownUI::Update()
{
    m_canvas->Update();
}

void CountdownUI::Render(RenderContext& rc)
{
    m_canvas->Render(rc);
}

void CountdownUI::SetState(State state)
{
    // 状態が同じなら何もしない
    if (m_currentState == state) return;

    m_currentState = state;
    m_countdown->isDraw = true;

    // 状態に合わせて画像とサイズを切り替える
    switch (state)
    {
    case State::READY:
        m_countdown->Initialize(TEX_READY, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NUM_5:
        m_countdown->Initialize(TEX_5, SIZE_NUM.x, SIZE_NUM.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NUM_4:
        m_countdown->Initialize(TEX_4, SIZE_NUM.x, SIZE_NUM.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NUM_3:
        m_countdown->Initialize(TEX_3, SIZE_NUM.x, SIZE_NUM.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NUM_2:
        m_countdown->Initialize(TEX_2, SIZE_NUM.x, SIZE_NUM.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NUM_1:
        m_countdown->Initialize(TEX_1, SIZE_NUM.x, SIZE_NUM.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::GO:
        m_countdown->Initialize(TEX_GO, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
        break;
    case State::NONE:
        m_countdown->isDraw = false;
        break;
    }
}