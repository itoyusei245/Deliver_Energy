/**
 * @file GameTimeUI.h
 * @brief ゲーム内経過時間（温度）表示UIのクラス定義
 */
#pragma once
#include "UIBase.h"

class GameTimeUI : public IGameObject
{
public:
    GameTimeUI();
    ~GameTimeUI();

    void Update() override;
    void Render(RenderContext& rc) override;

private:
    UICanvas* m_canvas = nullptr;

    UINumber* m_leftNumber = nullptr;  // 整数部（4, 5, 12 など）
    UIIcon* m_separator = nullptr;   // 小数点（ドット）
    UINumber* m_rightNumber = nullptr; // 小数部（0～9）
    UIIcon* m_suffix = nullptr;      // 単位アイコン（℃）

    float m_timer = 0.0f;
};