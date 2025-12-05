#include "stdafx.h"
#include "Pause.h"
#include "Game.h" // ゲーム中かどうかの判定用


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


    /**スタートボタンが押されたらフラグを反転（ON <->OFF）*/
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
                m_selectBar = 0; /**または 3 にしてループさせる*/
            }
        }
        if (g_pad[0]->IsTrigger(enButtonDown))
        {
            m_selectBar++;
            if (m_selectBar > 3) {
                m_selectBar = 3; /**または 0 にしてループさせる */
            }
        }


        /**現在選択されている画像だけ更新*/
        m_selectSprute[m_selectBar].Update();


        /**決定ボタン処理の枠組み*/
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            // 例: 
            // if (m_currentSelect == 0) { ... } // ゲーム再開
            // if (m_currentSelect == 3) { ... } // タイトルへ戻る 等
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