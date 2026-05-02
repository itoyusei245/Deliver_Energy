#include "stdafx.h"
#include "GameResult.h"
#include "Game.h"
#include "Title.h"
#include "Sound/SoundManager.h"
#include "Player.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss.h"
#include "Enemy/BossStatusUI.h"
#include "GetItem.h"
#include "Athletic/AthleticManager.h"
#include "Athletic/AthleticStage.h"
#include "Athletic/AthleticTrigger.h"
#include "StageManager.h"
#include <iomanip>
#include <sstream>

GameResult::GameResult()
{
    m_ui = new GameResultUI();

    SoundManager::Get().PlayBGM(enSoundKind_Result);
    CalculateScore();
}

GameResult::~GameResult()
{
    delete m_ui;
}

void GameResult::CalculateScore()
{
    // 1. 温度 (Float)
    float temp = Game::FinalTemperature;
    m_items[0].type = ScoreItem::TYPE_FLOAT;
    m_items[0].floatVal = temp;

    float tempOver = max(0.0f, temp - 4.0f);
    int tempPenalty = (int)(tempOver / 0.1f) * 4000;
    m_items[0].score = max(200000, 800000 - tempPenalty);

    if (temp <= 6.0f) m_items[0].rank = 'S';
    else if (temp <= 10.0f) m_items[0].rank = 'A';
    else if (temp <= 12.5f) m_items[0].rank = 'B';
    else if (temp <= 15.0f) m_items[0].rank = 'C';
    else m_items[0].rank = 'D';

    // 2. 撃破数 (Int)
    m_items[1].type = ScoreItem::TYPE_INT;
    m_items[1].intVal = Game::DefeatedEnemyCount;
    m_items[1].score = Game::DefeatedEnemyCount * 40000;

    int kill = Game::DefeatedEnemyCount;
    if (kill >= 25) m_items[1].rank = 'S';
    else if (kill >= 18) m_items[1].rank = 'A';
    else if (kill >= 13) m_items[1].rank = 'B';
    else if (kill >= 10) m_items[1].rank = 'C';
    else m_items[1].rank = 'D';

    // 3. HP (Int)
    m_items[2].type = ScoreItem::TYPE_INT;
    m_items[2].intVal = (int)Game::FinalHP;

    int hp = (int)Game::FinalHP;
    int hpPenalty = (100 - hp) * 2000;
    m_items[2].score = max(0, 200000 - hpPenalty);

    if (hp >= 90) m_items[2].rank = 'S';
    else if (hp >= 70) m_items[2].rank = 'A';
    else if (hp >= 50) m_items[2].rank = 'B';
    else if (hp >= 30) m_items[2].rank = 'C';
    else m_items[2].rank = 'D';

    // 4. コイン (Int)
    m_items[3].type = ScoreItem::TYPE_INT;
    m_items[3].intVal = Game::CoinCount;
    m_items[3].score = Game::CoinCount * 6000;

    int coin = Game::CoinCount;
    if (coin >= 120) m_items[3].rank = 'S';
    else if (coin >= 105) m_items[3].rank = 'A';
    else if (coin >= 90) m_items[3].rank = 'B';
    else if (coin >= 80) m_items[3].rank = 'C';
    else m_items[3].rank = 'D';

    // 5. タイム (Time)
    m_items[4].type = ScoreItem::TYPE_TIME;
    m_items[4].floatVal = Game::ClearTime;

    int timeSec = (int)Game::ClearTime;
    int timePenalty = timeSec * 1000;
    m_items[4].score = max(200000, 800000 - timePenalty);

    if (timeSec <= 60) m_items[4].rank = 'S';
    else if (timeSec <= 150) m_items[4].rank = 'A';
    else if (timeSec <= 300) m_items[4].rank = 'B';
    else if (timeSec <= 450) m_items[4].rank = 'C';
    else m_items[4].rank = 'D';

    // 合計
    m_totalScore = 0;
    for (int i = 0; i < 5; i++) m_totalScore += m_items[i].score;
}

void GameResult::Update()
{
    // --- 既存のカウントアップ演出ロジック（そのまま） ---
    if (m_animPhase == 0) {
        ScoreItem& item = m_items[m_activeItemIndex];
        if (item.currentDisp < item.score) {
            int step = max(3333, (item.score - item.currentDisp) / 10);
            item.currentDisp += step;
            if (item.currentDisp >= item.score) {
                item.currentDisp = item.score;
            }
        }
        else {
            m_activeItemIndex++;
            if (m_activeItemIndex >= 5) {
                m_animPhase = 1;
            }
        }
    }
    else if (m_animPhase == 1) {
        if (m_totalScoreDisp < m_totalScore) {
            int step = max(11111, (m_totalScore - m_totalScoreDisp) / 10);
            m_totalScoreDisp += step;
            if (m_totalScoreDisp > m_totalScore) m_totalScoreDisp = m_totalScore;
        }
        else {
            m_animPhase = 2;
        }
    }
    else if (m_animPhase == 2) {
        if (g_pad[0]->IsTrigger(enButtonA)) {
            SoundManager::Get().PlaySE(enSoundKind_Decision);
            EnemyManager::DeleteInstance(); AthleticManager::DeleteInstance(); StageManager::DeleteInstance(); GetItem::ResetCoinCount();
            DeleteGO(FindGO<Player>("player")); DeleteGO(FindGO<Boss>("boss")); DeleteGO(FindGO<BossStatusUI>("bossUI"));
            DeleteGO(FindGO<AthleticStage>("athleticStage")); DeleteGO(FindGO<AthleticTrigger>("athleticTriggerLoader"));
            DeleteGO(FindGO<Game>("game"));
            NewGO<Game>(0, "game"); DeleteGO(this);
        }
        else if (g_pad[0]->IsTrigger(enButtonB)) {
            SoundManager::Get().PlaySE(enSoundKind_Decision);
            EnemyManager::DeleteInstance(); AthleticManager::DeleteInstance(); StageManager::DeleteInstance(); GetItem::ResetCoinCount();
            DeleteGO(FindGO<Player>("player")); DeleteGO(FindGO<Boss>("boss")); DeleteGO(FindGO<BossStatusUI>("bossUI"));
            DeleteGO(FindGO<AthleticStage>("athleticStage")); DeleteGO(FindGO<AthleticTrigger>("athleticTriggerLoader"));
            DeleteGO(FindGO<Game>("game"));
            NewGO<Title>(0, "title"); DeleteGO(this);
        }
    }

    if (m_animPhase < 2 && g_pad[0]->IsTrigger(enButtonA)) {
        for (int i = 0; i < 5; i++) m_items[i].currentDisp = m_items[i].score;
        m_totalScoreDisp = m_totalScore;
        m_activeItemIndex = 5;
        m_animPhase = 2;
    }

    // ==========================================
    // UIクラスへ情報を渡して更新を依頼する
    // ==========================================
    m_ui->SetAnimPhase(m_activeItemIndex, m_animPhase);

    for (int i = 0; i < 5; i++) {
        if (i > m_activeItemIndex) continue;

        // 1. 左側の文字列を作成してUIに渡す
        std::stringstream ssVal;
        if (m_items[i].type == ScoreItem::TYPE_TIME) {
            int totalSec = (int)m_items[i].floatVal;
            int min = totalSec / 60;
            int sec = totalSec % 60;
            int msec = (int)((m_items[i].floatVal - totalSec) * 100);
            ssVal << std::setfill('0') << std::setw(2) << min << ":"
                << std::setfill('0') << std::setw(2) << sec << "."
                << std::setfill('0') << std::setw(2) << msec;
        }
        else if (m_items[i].type == ScoreItem::TYPE_FLOAT) {
            ssVal << std::fixed << std::setprecision(1) << m_items[i].floatVal;
        }
        else {
            ssVal << m_items[i].intVal;
        }
        m_ui->SetRowValueString(i, ssVal.str());

        // 2. スコア（数字）をUIに渡す
        m_ui->SetRowScore(i, m_items[i].currentDisp);

        // 3. カウントアップ終了後ならランクをUIに渡す
        if (m_items[i].currentDisp == m_items[i].score) {
            m_ui->SetRowRank(i, m_items[i].rank);
        }
    }

    if (m_animPhase >= 1) {
        m_ui->SetTotalScore(m_totalScoreDisp);
    }

    // UIのUpdateを呼び出す
    m_ui->Update();
}

void GameResult::Render(RenderContext& rc)
{
    m_ui->Render(rc);
}