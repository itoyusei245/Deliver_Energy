/**
 * @file CountdownUI.h
 * @brief カウントダウンの描画のみを担当するUIクラス
 */
#pragma once
#include "UIBase.h"

class CountdownUI
{
public:
    // 表示する状態の定義
    enum class State {
        READY,
        NUM_5,
        NUM_4,
        NUM_3,
        NUM_2,
        NUM_1,
        GO,
        NONE // 非表示
    };

    CountdownUI();
    ~CountdownUI();

    void Update();
    void Render(RenderContext& rc);

    // ロジック側から状態をセットする関数
    void SetState(State state);

private:
    UICanvas* m_canvas = nullptr;
    UIIcon* m_operation = nullptr; // 操作説明背景
    UIIcon* m_countdown = nullptr; // カウントダウン文字

    State m_currentState = State::NONE;
};