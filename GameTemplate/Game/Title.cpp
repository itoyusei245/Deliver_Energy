/**
 * @file Title.cpp
 * @brief タイトル画面の実装
 */
#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Setting.h"
#include "system/system.h"
#include "Sound/SoundManager.h"
#include "GameLoading.h"

 // 静的変数の実体定義
bool Title::IsTitle = false;
bool Title::IsSetting = false;

namespace {
    // --- マジックナンバー・ストリングの定数化 ---
    constexpr const char* NAME_LOADING = "gameLoading";
    constexpr const char* NAME_SETTING = "setting";

    // メニューのインデックス
    constexpr int MENU_START = 0;
    constexpr int MENU_SETTING = 1;
    constexpr int MENU_QUIT = 2;
}

Title::Title()
{
    // UIの生成
    m_ui = new TitleUI();
    m_currentBar = MENU_START;

    // UIに初期状態を伝達
    m_ui->SetCurrentBar(m_currentBar);

    SoundManager::Get().PlayBGM(enSoundKind_Title);
}

Title::~Title()
{
    delete m_ui;
}

bool Title::Start()
{
    IsTitle = false;
    IsSetting = false;
    return true;
}

void Title::Update()
{
    // 設定画面を開いている間は入力を受け付けない
    if (IsSetting) {
        return;
    }

    UpdatePicUp();

    if (g_pad[0]->IsTrigger(enButtonA))
    {
        SoundManager::Get().PlaySE(enSoundKind_Decision);

        if (m_currentBar == MENU_START) {
            // ロード画面へ
            NewGO<GameLoading>(0, NAME_LOADING);
            DeleteGO(this);
        }
        else if (m_currentBar == MENU_SETTING) {
            // 設定を開く
            IsSetting = true;
            NewGO<Setting>(0, NAME_SETTING);
        }
        else if (m_currentBar == MENU_QUIT) {
            // ゲームを終了する
            g_gameLoop.m_isLoop = false;
        }
    }

    // UIの更新
    m_ui->Update();
}

void Title::UpdatePicUp()
{
    if (g_pad[0]->IsTrigger(enButtonUp))
    {
        SoundManager::Get().PlaySE(enSoundKind_Choose);
        m_currentBar--;
        if (m_currentBar < MENU_START) {
            m_currentBar = MENU_START;
        }
        m_ui->SetCurrentBar(m_currentBar);
    }

    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        SoundManager::Get().PlaySE(enSoundKind_Choose);
        m_currentBar++;
        if (m_currentBar > MENU_QUIT) {
            m_currentBar = MENU_QUIT;
        }
        m_ui->SetCurrentBar(m_currentBar);
    }
}

void Title::Render(RenderContext& rc)
{
    // 描画はすべてUIに任せる
    m_ui->Render(rc);
}