/**
 * @file TitleUI.cpp
 * @brief タイトル画面UIの実装
 */
#include "stdafx.h"
#include "TitleUI.h"

namespace {
    // --- 画像パス ---
    constexpr const char* PATH_TITLE_BG = "Assets/sprite/Title.DDS";

    constexpr const char* PATH_SELECT[3] = {
        "Assets/sprite/selectBer_GameState.DDS",
        "Assets/sprite/selectBer_Settings.DDS",
        "Assets/sprite/selectBer_Quit.DDS"
    };

    // --- サイズと座標 ---
    const Vector2 SIZE_FULL(1920.0f, 1080.0f);
    const Vector3 POS_ZERO(0.0f, 0.0f, 0.0f);
}

TitleUI::TitleUI()
{
    m_canvas = new UICanvas();

    // 背景の初期化
    m_titleBg = m_canvas->CreateUI<UIIcon>();
    m_titleBg->Initialize(PATH_TITLE_BG, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

    // 選択肢画像の初期化
    m_selectIcon = m_canvas->CreateUI<UIIcon>();
    m_selectIcon->Initialize(PATH_SELECT[0], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
}

TitleUI::~TitleUI()
{
    delete m_canvas;
}

void TitleUI::Update()
{
    m_canvas->Update();
}

void TitleUI::Render(RenderContext& rc)
{
    m_canvas->Render(rc);
}

void TitleUI::SetCurrentBar(int index)
{
    // 番号が変わった時だけ画像を切り替える
    if (m_currentBarIndex != index && index >= 0 && index < 3) {
        m_currentBarIndex = index;
        m_selectIcon->Initialize(PATH_SELECT[index], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
    }
}