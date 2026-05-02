/**
 * @file TitleUI.h
 * @brief タイトル画面の描画のみを担当するUIクラス
 */
#pragma once
#include "UIBase.h"

class TitleUI
{
public:
    TitleUI();
    ~TitleUI();

    void Update();
    void Render(RenderContext& rc);

    void SetCurrentBar(int index);

private:
    UICanvas* m_canvas = nullptr;

    UIIcon* m_titleBg = nullptr;    // タイトル背景画像
    UIIcon* m_selectIcon = nullptr; // 選択肢のハイライト画像

    int m_currentBarIndex = -1;
};