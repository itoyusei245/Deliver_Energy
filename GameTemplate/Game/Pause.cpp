#include "stdafx.h"
#include "Pause.h"
#include "Game.h"
#include "Title.h"
// 削除対象のクラスを知るためにインクルードが必要
#include "CoinUI.h"
#include "GameTimeUI.h"
#include "Countdown.h"

Pause::Pause()
{
    m_filterSprite.Init("Assets/sprite/pause.DDS", 1920.0f, 1080.0f);
    m_filterSprite.SetPosition(Vector3::Zero);
    m_filterSprite.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.8f });

    m_pauseSprite.Init(TEX_PAESE, 1920.0f, 1080.0f);
    m_pauseSprite.SetPosition(Vector3::Zero);

    for (int i = 0; i < 4; i++)
    {
        m_selectSprute[i].Init(TEX_SELECT[i], 1920.0f, 1080.0f);
        m_selectSprute[i].SetPosition(Vector3::Zero);
    }

    m_isActive = false;
    m_selectBar = 0;
}

Pause::~Pause()
{
}

void Pause::Update()
{
    /**ゲームプレイ中（カウントダウン後）でなければ処理しない*/
    if (!Game::IsGamePlay) return;

    /**スタートボタンが押されたらフラグを反転（ON <-> OFF）*/
    if (g_pad[0]->IsTrigger(enButtonStart))
    {
        m_isActive = !m_isActive;
        Game::IsPaused = m_isActive;
    }

    /**表示中なら更新*/
    if (m_isActive)
    {
        m_filterSprite.Update();
        m_pauseSprite.Update();

        if (g_pad[0]->IsTrigger(enButtonUp))
        {
            m_selectBar--;
            if (m_selectBar < 0) {
                m_selectBar = 3; /**ループさせる*/
            }
        }
        if (g_pad[0]->IsTrigger(enButtonDown))
        {
            m_selectBar++;
            if (m_selectBar > 3) {
                m_selectBar = 0; /**ループさせる*/
            }
        }

        /**現在選択されている画像だけ更新*/
        m_selectSprute[m_selectBar].Update();

        /**決定ボタン処理*/
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            if (m_selectBar == 0)
            {
                // 0: そのままゲームを続ける
                m_isActive = false;
                Game::IsPaused = false;
            }
            else if (m_selectBar == 1)
            {
                // 1: リトライ（カウントダウンから始める）

                // 次のゲームのためにポーズフラグを解除
                Game::IsPaused = false;

                // Gameクラスのデストラクタで消えないUI類をここで手動削除
                // これを忘れるとリトライのたびにUIが重なって重くなる
                DeleteGO(FindGO<CoinUI>("coinUI"));
                DeleteGO(FindGO<GameTimeUI>("gameTimeUI"));
                DeleteGO(FindGO<Countdown>("countdown"));

                // 現在のゲーム本体を削除（Player, Camera, Manager等はここで消える）
                DeleteGO(FindGO<Game>("game"));

                // 新しいゲームを作成
                NewGO<Game>(0, "game");

                // 最後に自分自身（古いPause）を削除
                DeleteGO(this);
            }
            else if (m_selectBar == 2)
            {
                // 2: 設定（いったんなし）
                // 何もしない
            }
            else if (m_selectBar == 3)
            {
                // 3: タイトルに戻る

                // ポーズフラグを解除
                Game::IsPaused = false;

                // ゲーム関連のオブジェクトを全て削除
                DeleteGO(FindGO<Game>("game"));
                DeleteGO(FindGO<CoinUI>("coinUI"));
                DeleteGO(FindGO<GameTimeUI>("gameTimeUI"));
                DeleteGO(FindGO<Countdown>("countdown"));

                // タイトル画面を作成
                NewGO<Title>(0, "title");

                // 自分自身を削除
                DeleteGO(this);
            }
        }
    }
}

void Pause::Render(RenderContext& rc)
{
    /**ゲームプレイ中 かつ アクティブな時だけ描画*/
    if (Game::IsGamePlay && m_isActive)
    {
        m_filterSprite.Draw(rc);
        m_pauseSprite.Draw(rc);
        m_selectSprute[m_selectBar].Draw(rc);
    };
}