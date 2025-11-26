#include "stdafx.h"
#include "Countdown.h"

Countdown::Countdown()
{
    /**最初に「3」の画像で初期化（サイズは適宜調整してください）*/
    m_spriteRender.Init(TEX_3, 500.0f, 500.0f);

    /**画面中央に配置*/
    m_spriteRender.SetPosition(Vector3::Zero);

    /**タイマーリセット*/
    m_timer = 0.0f;
}

Countdown::~Countdown()
{
}

void Countdown::Update()
{
    /**時間を進める*/
    m_timer += g_gameTime->GetFrameDeltaTime();

    if (m_timer < 1.5f) {
        m_spriteRender.Init(TEX_READY, 1920.0f, 1080.0f);
    }
    else if (m_timer < 2.5f) {
        m_spriteRender.Init(TEX_5, 100.0f, 100.0f);
    }
    else if (m_timer < 3.5f) {
        m_spriteRender.Init(TEX_4, 100.0f, 100.0f);
    }
    else if (m_timer < 4.5f) {
        m_spriteRender.Init(TEX_3, 100.0f, 100.0f);
    }
    else if (m_timer < 5.5f) {
        m_spriteRender.Init(TEX_2, 100.0f, 100.0f);
    }
    else if (m_timer < 6.5f) {
        m_spriteRender.Init(TEX_1, 100.0f, 100.0f);
    }
    else if (m_timer < 8.5f) {
        m_spriteRender.Init(TEX_GO, 1920.0f, 1080.0f);
    }
    else {
        /**8.5秒経ったら終了*/
        m_isFinished = true;
        /**自身を削除（表示を消すため）*/
        DeleteGO(this);
    }

    m_spriteRender.Update();
}

void Countdown::Render(RenderContext& rc)
{
    /**まだ終わってなければ描画*/
    if (!m_isFinished) {
        m_spriteRender.Draw(rc);
    }
}