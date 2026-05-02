/**
 * @file Countdown.cpp
 * @brief カウントダウン演出の実装
 */
#include "stdafx.h"
#include "Countdown.h"

Countdown::Countdown()
{
    m_ui = new CountdownUI();
    m_timer = 0.0f;
    m_isFinished = false;
}

Countdown::~Countdown()
{
    delete m_ui;
}

void Countdown::Update()
{
    m_timer += g_gameTime->GetFrameDeltaTime();

    // 経過時間に応じてUIに状態をセットする
    if (m_timer < 1.5f) {
        m_ui->SetState(CountdownUI::State::READY);
    }
    else if (m_timer < 2.5f) {
        m_ui->SetState(CountdownUI::State::NUM_5);
    }
    else if (m_timer < 3.5f) {
        m_ui->SetState(CountdownUI::State::NUM_4);
    }
    else if (m_timer < 4.5f) {
        m_ui->SetState(CountdownUI::State::NUM_3);
    }
    else if (m_timer < 5.5f) {
        m_ui->SetState(CountdownUI::State::NUM_2);
    }
    else if (m_timer < 6.5f) {
        m_ui->SetState(CountdownUI::State::NUM_1);
    }
    else if (m_timer < 8.5f) {
        m_ui->SetState(CountdownUI::State::GO);
    }
    else {
        m_isFinished = true;
        DeleteGO(this);
    }

    if (!m_isFinished) {
        m_ui->Update();
    }
}

void Countdown::Render(RenderContext& rc)
{
    if (!m_isFinished) {
        m_ui->Render(rc);
    }
}