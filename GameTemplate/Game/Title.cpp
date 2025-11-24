#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include"system/system.h"

/**
 * @brief Titleクラスのコンストラクタ
 * @details タイトル画面用のスプライト画像を初期化します。
 */
Title::Title()
{
    //! タイトル画面の画像を読み込み
    m_titleSprite.Init("Assets/sprite/Title.DDS", 1920.0f, 1080.0f);
    m_picUpSprite[0].Init("Assets/sprite/selectBer_GameState.DDS", 1920.0f, 1080.0f);
    m_picUpSprite[1].Init("Assets/sprite/selectBer_Settings.DDS", 1920.0f, 1080.0f);
    m_picUpSprite[2].Init("Assets/sprite/selectBer_Quit.DDS", 1920.0f, 1080.0f);
}

/**
 * @brief Titleクラスのデストラクタ
 */
Title::~Title()
{
	
}

/**
 * @brief 毎フレームの更新処理
 * @details Aボタンが押されたらゲーム画面へ遷移し、自身を削除します。
 */
void Title::Update()
{
    UpdatePicUp();
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        /**ゲームを始める*/
        if (m_currentBar == 0) {
            //! ゲームのオブジェクトを作成
            NewGO<Game>(0, "game");
            //! 自身を削除する
            DeleteGO(this);
        }
        /**設定を開く*/
        if (m_currentBar == 1) {
            //! ゲームのオブジェクトを作成
            NewGO<Game>(0, "game");
            //! 自身を削除する
            DeleteGO(this);
        }
        /**ゲームを終了する*/
        if (m_currentBar == 2) {
            g_gameLoop.m_isLoop = false;
        }

    }
}

void Title::UpdatePicUp()
{
 
    if (g_pad[0]->IsTrigger(enButtonUp))
    {
        if(m_currentBar==0){
            m_currentBar = 0;
        }
        else {
            m_currentBar = m_currentBar - 1.0f;
        }
    }
    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        if (m_currentBar == 2/*カレントが最大値の時*/) {
            m_currentBar = 2;
        }
        else {
            m_currentBar = m_currentBar + 1.0f;
        }
    }
}

/**
 * @brief タイトル画面の描画処理
 * @param rc 描画コンテキスト
 * @details タイトル画像の描画を行います。
 */
void Title::Render(RenderContext& rc)
{
    m_titleSprite.Draw(rc);
    m_picUpSprite[m_currentBar].Draw(rc);
}