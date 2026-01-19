/**
 * @file Countdown.cpp
 * @brief カウントダウン演出の実装
 */
#include "stdafx.h"
#include "Countdown.h"

 /**
  * @brief コンストラクタ
  * @details 操作説明画像とカウントダウン用スプライトの初期化を行います。
  */
Countdown::Countdown()
{
    // 操作説明画像の初期化（画面全体 1920x1080）
    m_operation.Init(TEX_OPERATION, 1920.0f, 1080.0f);
    m_operation.SetPosition(Vector3::Zero);

    // カウントダウン画像の初期化
    m_countdown.Init(TEX_3, 500.0f, 500.0f);
    /**画面中央に配置*/
    m_countdown.SetPosition(Vector3::Zero);

    /**タイマーリセット*/
    m_timer = 0.0f;

    m_currentState = -1;
}

Countdown::~Countdown()
{
}

/**
 * @brief 更新処理
 * @details
 * 経過時間(m_timer)に応じて表示する画像を切り替えます。
 * - 0.0s ～ 1.5s : Ready
 * - 1.5s ～ 2.5s : 5
 * - ...
 * - 6.5s ～ 8.5s : GO
 * - 8.5s以降 : 演出終了とし、自身を削除(DeleteGO)します。
 */
void Countdown::Update()
{
    /**時間を進める*/
    m_timer += g_gameTime->GetFrameDeltaTime();

    if (m_timer < 1.5f) {
        // "Ready" 表示
        m_countdown.Init(TEX_READY, 1920.0f, 1080.0f);
    }
    else if (m_timer < 2.5f) {
        // "5" 表示
        m_countdown.Init(TEX_5, 100.0f, 100.0f);
    }
    else if (m_timer < 3.5f) {
        // "4" 表示
        m_countdown.Init(TEX_4, 100.0f, 100.0f);
    }
    else if (m_timer < 4.5f) {
        // "3" 表示
        m_countdown.Init(TEX_3, 100.0f, 100.0f);
    }
    else if (m_timer < 5.5f) {
        // "2" 表示
        m_countdown.Init(TEX_2, 100.0f, 100.0f);
    }
    else if (m_timer < 6.5f) {
        // "1" 表示
        m_countdown.Init(TEX_1, 100.0f, 100.0f);
    }
    else if (m_timer < 8.5f) {
        // "GO" 表示
        m_countdown.Init(TEX_GO, 1920.0f, 1080.0f);
    }
    else {
        /**8.5秒経ったら終了*/
        m_isFinished = true;

        /**自身を削除（表示を消すため、メモリ解放のため）*/
        DeleteGO(this);
    }

    // 終了していなければスプライトの更新を行う
    if (!m_isFinished) {
        m_operation.Update();
        m_countdown.Update();
    }
}

/**
 * @brief 描画処理
 */
void Countdown::Render(RenderContext& rc)
{
    /**まだ終わってなければ描画*/
    if (!m_isFinished) {
        m_operation.Draw(rc);
        m_countdown.Draw(rc);
    }
}