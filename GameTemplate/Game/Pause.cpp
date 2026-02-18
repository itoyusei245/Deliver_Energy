/**
 * @file Pause.cpp
 * @brief ポーズ機能の実装
 */
#include "stdafx.h"
#include "Pause.h"
#include "Game.h" 
#include "Setting.h"
#include "Title.h"
#include "CoinUI.h"
#include "GameTimeUI.h"
#include "Countdown.h"

Pause::Pause()
{
    // フィルター画像の初期化（半透明の黒）
    m_filterSprite.Init("Assets/sprite/pause.DDS", 1920.0f, 1080.0f);
    m_filterSprite.SetPosition(Vector3::Zero);
    m_filterSprite.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.8f });

    // PAUSEロゴと選択肢画像の初期化
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

/**
 * @brief 更新処理
 * @details
 * - スタートボタンでポーズのON/OFFを切り替えます。
 * - ポーズ中は十字キーでメニュー選択、Aボタンで決定します。
 * - リトライやタイトルの場合、既存のゲームオブジェクトを検索して削除し、状態をリセットします。
 */
void Pause::Update()
{
    /**ゲームプレイ中（カウントダウン後）でなければ処理しない*/
    if (!Game::IsGamePlay) return;

    /**スタートボタンが押されたらフラグを反転（ON <-> OFF）*/
    if (g_pad[0]->IsTrigger(enButtonStart))
    {
        m_isActive = !m_isActive;
        Game::IsPaused = m_isActive; // 静的フラグを更新して他のオブジェクトを停止させる
        // ポーズを解除したとき、もし設定画面が開いていたら強制的に閉じる
        if (!m_isActive) {
            Setting* setting = FindGO<Setting>("setting");
            if (setting != nullptr) {
                DeleteGO(setting);
            }
        }
    }

    /**表示中ならメニュー操作を受け付ける*/
    if (m_isActive)
    {
        // 設定画面が開いている間は、ポーズ画面の操作を受け付けない
        if (FindGO<Setting>("setting") != nullptr) {
            return;
        }

        m_filterSprite.Update();
        m_pauseSprite.Update();

        // メニュー選択（ループ移動）
        if (g_pad[0]->IsTrigger(enButtonUp))
        {
            m_selectBar--;
            if (m_selectBar < 0) m_selectBar = 3;
        }
        if (g_pad[0]->IsTrigger(enButtonDown))
        {
            m_selectBar++;
            if (m_selectBar > 3) m_selectBar = 0;
        }

        /**現在選択されている画像だけ更新*/
        m_selectSprute[m_selectBar].Update();

        /**決定ボタン処理*/
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            if (m_selectBar == 0)
            {
                // 0: Continue（再開）
                m_isActive = false;
                Game::IsPaused = false;
            }
            else if (m_selectBar == 1)
            {
                // 1: Retry（リトライ）
                // ポーズ解除
                Game::IsPaused = false;

                // UI類を手動削除（二重生成防止）
                DeleteGO(FindGO<CoinUI>("coinUI"));
                DeleteGO(FindGO<GameTimeUI>("gameTimeUI"));
                DeleteGO(FindGO<Countdown>("countdown"));

                // ゲーム本体を削除（Player等はGameのデストラクタで消える想定）
                DeleteGO(FindGO<Game>("game"));

                // 新しいゲームを作成
                NewGO<Game>(0, "game");

                // 自身を削除
                DeleteGO(this);
            }
            else if (m_selectBar == 2)
            {
                NewGO<Setting>(0, "setting");

            }
            else if (m_selectBar == 3)
            {
                // 3: Title（タイトルへ戻る）
                Game::IsPaused = false;

                // 全オブジェクト削除
                DeleteGO(FindGO<Game>("game"));
                DeleteGO(FindGO<CoinUI>("coinUI"));
                DeleteGO(FindGO<GameTimeUI>("gameTimeUI"));
                DeleteGO(FindGO<Countdown>("countdown"));

                // タイトル画面生成
                NewGO<Title>(0, "title");

                // 自身削除
                DeleteGO(this);
            }
        }
    }
}

void Pause::Render(RenderContext& rc)
{
    /**ゲームプレイ中 かつ ポーズ中のみ描画*/
    if (Game::IsGamePlay && m_isActive)
    {
        m_filterSprite.Draw(rc);
        m_pauseSprite.Draw(rc);
        m_selectSprute[m_selectBar].Draw(rc);
    };
}