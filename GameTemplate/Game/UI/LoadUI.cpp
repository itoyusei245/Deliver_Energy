#include "stdafx.h"
#include "LoadUI.h"

namespace {
    const char* const LOAD_TEX_PATH = "Assets/sprite/LoadingUI.DDS";
    const float       LOAD_TEX_W = 300.0f;
    const float       LOAD_TEX_H = 60.0f;
    const Vector3     LOAD_POS = { 750.0f, -450.0f, 0.0f };
}

LoadUI::LoadUI()
{
    // キャンバスを作成し、その中に画像(Icon)を作成する
    m_canvas = new UICanvas();
    m_icon = m_canvas->CreateUI<UIIcon>();
}

LoadUI::~LoadUI()
{
    // キャンバスを消せば、中のアイコンも自動的に安全に消去されます！
    delete m_canvas;
}

bool LoadUI::Start()
{
    // 画像パス、幅、高さ、座標 を一気に指定して初期化
    m_icon->Initialize(
        LOAD_TEX_PATH,
        LOAD_TEX_W,
        LOAD_TEX_H,
        LOAD_POS,
        Vector3::One,            // スケール
        Quaternion::Identity     // 回転
    );

    return true;
}

void LoadUI::Update()
{
    m_canvas->Update();
}

void LoadUI::Render(RenderContext& rc)
{
    m_canvas->Render(rc);
}