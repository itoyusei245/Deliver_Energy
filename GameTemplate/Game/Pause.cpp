#include "stdafx.h"
#include "Pause.h"
#include "Game.h" // ゲーム中かどうかの判定用


Pause::Pause()
{
    // ★変更：真っ白な画像を読み込む
    m_filterSprite.Init("Assets/sprite/pause.DDS", 1920.0f, 1080.0f);

    // 画面中央
    m_filterSprite.SetPosition(Vector3::Zero);

    // ★重要：ここで「色」と「透明度」をいじる
    // 引数は (Red, Green, Blue, Alpha)
    // 黒(0,0,0) で、透明度50%(0.5) に設定
    m_filterSprite.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.8f });

    // ※もし SetMulColor がなければ SetColor を試してください

    m_isActive = false;
}


Pause::~Pause()
{
}


void Pause::Update()
{
    // ゲームプレイ中（カウントダウン後）でなければ処理しない
    if (!Game::IsGamePlay) return;

    // スタートボタンが押されたらフラグを反転（ON <-> OFF）
    if (g_pad[0]->IsTrigger(enButtonStart))
    {
        m_isActive = !m_isActive;

        Game::IsPaused = m_isActive;
    }

    // 表示中なら更新
    if (m_isActive)
    {
        m_filterSprite.Update();
    }
}


void Pause::Render(RenderContext& rc)
{
    // ゲームプレイ中 かつ アクティブな時だけ描画
    if (Game::IsGamePlay && m_isActive)
    {
        m_filterSprite.Draw(rc);
    }
}