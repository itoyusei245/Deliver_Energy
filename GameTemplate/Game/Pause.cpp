/**
 * @file Pause.cpp
 * @brief ポーズ機能の実装
 */
#include "stdafx.h"
#include "Pause.h"
#include "Game.h" 
#include "Setting.h"
#include "Title.h"
#include "UI/CoinUI.h"
#include "UI/GameTimeUI.h"
#include "Countdown.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss.h"
#include "Enemy/BossStatusUI.h"
#include "GetItem.h"
#include "Athletic/AthleticManager.h"
#include "Athletic/AthleticStage.h"
#include "Athletic/AthleticTrigger.h"
#include "StageManager.h"

namespace {
    // --- 検索・生成用マジックストリングの定数化 ---
    constexpr const char* NAME_SETTING   = "setting";
    constexpr const char* NAME_COIN_UI   = "coinUI";
    constexpr const char* NAME_TIME_UI   = "gameTimeUI";
    constexpr const char* NAME_COUNTDOWN = "countdown";
    constexpr const char* NAME_ATHLETIC  = "athleticStage";
    constexpr const char* NAME_TRIGGER   = "athleticTriggerLoader";
    constexpr const char* NAME_BOSS      = "boss";
    constexpr const char* NAME_BOSS_UI   = "bossUI";
    constexpr const char* NAME_GAME      = "game";
    constexpr const char* NAME_TITLE     = "title";
}

Pause::Pause()
{
    m_ui = new PauseUI();
    m_isActive = false;
    m_selectBar = 0;

    m_ui->SetActive(m_isActive);
    m_ui->SetSelectBar(m_selectBar);
}

Pause::~Pause()
{
    delete m_ui;
}

void Pause::Update()
{
    if (!Game::IsGamePlay) return;

    if (g_pad[0]->IsTrigger(enButtonStart))
    {
        m_isActive = !m_isActive;
        Game::IsPaused = m_isActive;

        m_ui->SetActive(m_isActive);

        if (!m_isActive) {
            Setting* setting = FindGO<Setting>(NAME_SETTING);
            if (setting != nullptr) {
                DeleteGO(setting);
            }
        }
    }

    if (m_isActive)
    {
        // 設定画面が開いている間は操作を受け付けない
        if (FindGO<Setting>(NAME_SETTING) != nullptr) {
            return;
        }

        if (g_pad[0]->IsTrigger(enButtonUp)) {
            m_selectBar--;
            if (m_selectBar < 0) m_selectBar = 3;
            m_ui->SetSelectBar(m_selectBar);
        }
        if (g_pad[0]->IsTrigger(enButtonDown)) {
            m_selectBar++;
            if (m_selectBar > 3) m_selectBar = 0;
            m_ui->SetSelectBar(m_selectBar);
        }

        // 決定ボタン処理
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            if (m_selectBar == 0)
            {
                m_isActive = false;
                Game::IsPaused = false;
                m_ui->SetActive(false);
            }
            else if (m_selectBar == 1) 
            {
                Game::IsPaused = false;

                EnemyManager::DeleteInstance();
                AthleticManager::DeleteInstance();
                StageManager::DeleteInstance();
                GetItem::ResetCoinCount();

                DeleteGO(FindGO<CoinUI>(NAME_COIN_UI));
                DeleteGO(FindGO<GameTimeUI>(NAME_TIME_UI));
                DeleteGO(FindGO<Countdown>(NAME_COUNTDOWN));
                DeleteGO(FindGO<AthleticStage>(NAME_ATHLETIC));
                DeleteGO(FindGO<AthleticTrigger>(NAME_TRIGGER));
                DeleteGO(FindGO<Boss>(NAME_BOSS));
                DeleteGO(FindGO<BossStatusUI>(NAME_BOSS_UI));
                DeleteGO(FindGO<Game>(NAME_GAME));

                NewGO<Game>(0, NAME_GAME);
                DeleteGO(this);
            }
            else if (m_selectBar == 2) 
            {
                NewGO<Setting>(0, NAME_SETTING);
            }
            else if (m_selectBar == 3)
            {
                Game::IsPaused = false;

                EnemyManager::DeleteInstance();
                AthleticManager::DeleteInstance();
                StageManager::DeleteInstance();
                GetItem::ResetCoinCount();

                DeleteGO(FindGO<Game>(NAME_GAME));
                DeleteGO(FindGO<CoinUI>(NAME_COIN_UI));
                DeleteGO(FindGO<GameTimeUI>(NAME_TIME_UI));
                DeleteGO(FindGO<Countdown>(NAME_COUNTDOWN));
                DeleteGO(FindGO<AthleticStage>(NAME_ATHLETIC));
                DeleteGO(FindGO<AthleticTrigger>(NAME_TRIGGER));
                DeleteGO(FindGO<Boss>(NAME_BOSS));
                DeleteGO(FindGO<BossStatusUI>(NAME_BOSS_UI));

                NewGO<Title>(0, NAME_TITLE);
                DeleteGO(this);
            }
        }
    }

    m_ui->Update();
}

void Pause::Render(RenderContext& rc)
{
    if (Game::IsGamePlay && m_isActive)
    {
        m_ui->Render(rc);
    }
}