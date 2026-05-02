/**
 * @file PauseUI.h
 * @brief ポーズ画面の描画のみを担当するUIクラス
 */
#pragma once
#include "UIBase.h"

class PauseUI
{
public:
    PauseUI();
    ~PauseUI();

    void Update();
    void Render(RenderContext& rc);

    //「表示ON/OFF」と「現在の選択番号」を受け取る
    void SetActive(bool isActive);
    void SetSelectBar(int selectIndex);

private:
    UICanvas* m_canvas = nullptr;

    UIIcon* m_filterIcon = nullptr; // 背景を暗くするフィルター
    UIIcon* m_pauseIcon = nullptr;  // "PAUSE" 文字画像
    UIIcon* m_selectIcon = nullptr; // 選択肢の画像（動的に切り替える）

    bool m_isActive = false;
    int m_currentSelectIndex = -1; // -1は未選択（初期状態）
};