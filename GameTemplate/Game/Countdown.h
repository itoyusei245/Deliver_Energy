/**
 * @file Countdown.h
 * @brief ゲーム開始時のカウントダウン演出クラス定義
 */
#pragma once
#include "UI/CountdownUI.h" 

class Countdown : public IGameObject
{
public:
    Countdown();
    ~Countdown();

    void Update() override;
    void Render(RenderContext& rc) override;

    bool IsFinished() const { return m_isFinished; }

private:
    CountdownUI* m_ui = nullptr;
    float m_timer = 0.0f;
    bool m_isFinished = false;
};