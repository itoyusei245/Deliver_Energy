/**
 * @file PauseUI.cpp
 * @brief ポーズ画面UIの実装
 */
#include "stdafx.h"
#include "PauseUI.h"

namespace {
    // --- 画像パス ---
    constexpr const char* PATH_FILTER = "Assets/sprite/pause.DDS";
    constexpr const char* PATH_PAUSE = "Assets/sprite/pauseSelect.DDS";

    // 選択肢の画像パス配列（0: Continue, 1: Retry, 2: Setting, 3: Title）
    constexpr const char* PATH_SELECT[4] = {
        "Assets/sprite/pauseSelect_Continue.DDS",
        "Assets/sprite/pauseSelect_Retry.DDS",
        "Assets/sprite/pauseSelect_Setting.DDS",
        "Assets/sprite/pauseSelect_Title.DDS"
    };

    // --- サイズと座標 ---
    const Vector2 SIZE_FULL(1920.0f, 1080.0f);
    const Vector3 POS_ZERO(0.0f, 0.0f, 0.0f);
    const Vector4 COLOR_FILTER(0.0f, 0.0f, 0.0f, 0.8f); 
}

PauseUI::PauseUI()
{
    m_canvas = new UICanvas();

    // 1. フィルター画像の初期化
    m_filterIcon = m_canvas->CreateUI<UIIcon>();
    m_filterIcon->Initialize(PATH_FILTER, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
    m_filterIcon->color_ = COLOR_FILTER;

    // 2. PAUSEロゴの初期化
    m_pauseIcon = m_canvas->CreateUI<UIIcon>();
    m_pauseIcon->Initialize(PATH_PAUSE, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

    // 3. 選択肢画像の初期化
    m_selectIcon = m_canvas->CreateUI<UIIcon>();
    m_selectIcon->Initialize(PATH_SELECT[0], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

    // 初期状態は非表示
    m_canvas->SetEnable(false);
}

PauseUI::~PauseUI()
{
    delete m_canvas;
}

void PauseUI::Update()
{
    if (m_isActive) {
        m_canvas->Update();
    }
}

void PauseUI::Render(RenderContext& rc)
{
    if (m_isActive) {
        m_canvas->Render(rc);
    }
}

void PauseUI::SetActive(bool isActive)
{
    m_isActive = isActive;
    m_canvas->SetEnable(isActive);
}

void PauseUI::SetSelectBar(int selectIndex)
{
    // 番号が変わった時だけ画像を切り替える
    if (m_currentSelectIndex != selectIndex && selectIndex >= 0 && selectIndex < 4) {
        m_currentSelectIndex = selectIndex;
        m_selectIcon->Initialize(PATH_SELECT[selectIndex], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
    }
}