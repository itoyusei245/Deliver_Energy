/**
 * @file GameResultUI.cpp
 * @brief リザルト画面の描画のみを担当するUIクラス
 */
#include "stdafx.h"
#include "GameResultUI.h"

 // --- レイアウト定数 ---
namespace Layout {
    const float ROW_START_Y = 190.0f;
    const float ROW_STEP_Y  = -83.0f;
    const float COL_VALUE_X = 400.0f;
    const float COL_SCORE_X = 420.0f;
    const float COL_RANK_X  = 675.0f;
    const float TOTAL_X     = 350.0f;
    const float TOTAL_Y     = -255.0f;

    const char* NUM_TEX[10] = {
        "Assets/sprite/0.DDS", "Assets/sprite/1.DDS", "Assets/sprite/2.DDS", "Assets/sprite/3.DDS", "Assets/sprite/4.DDS",
        "Assets/sprite/5.DDS", "Assets/sprite/6.DDS", "Assets/sprite/7.DDS", "Assets/sprite/8.DDS", "Assets/sprite/9.DDS"
    };
    const char* DOT_TEX = "Assets/sprite/dot.DDS";
    const char* RANK_TEX[5] = {
        "Assets/sprite/result/S.DDS", "Assets/sprite/result/A.DDS", "Assets/sprite/result/B.DDS", "Assets/sprite/result/C.DDS", "Assets/sprite/result/D.DDS"
    };
}

GameResultUI::GameResultUI()
{
    m_canvas = new UICanvas();

    // 背景とフレーム
    m_bg = m_canvas->CreateUI<UIIcon>();
    m_bg->Initialize("Assets/sprite/result/Result.DDS", 1920.0f, 1080.0f, Vector3::Zero, Vector3::One, Quaternion::Identity);

    m_scoreFrame = m_canvas->CreateUI<UIIcon>();
    m_scoreFrame->Initialize("Assets/sprite/result/ScoreDisplay_W.DDS", 1920.0f, 1080.0f, Vector3::Zero, Vector3::One, Quaternion::Identity);

    // 各行のUINumber初期化
    for (int i = 0; i < 5; i++) {
        float y = Layout::ROW_START_Y + (Layout::ROW_STEP_Y * i);

        m_rows[i].scoreNumber = m_canvas->CreateUI<UINumber>();
        m_rows[i].scoreNumber->Initialize("Assets/sprite/", ".DDS", 50.0f * 0.4f, 50.0f * 0.4f, Vector3(Layout::COL_SCORE_X, y, 0.0f), 20.0f);
        m_rows[i].scoreNumber->SetZeroPadding(false);
        m_rows[i].scoreNumber->isDraw = false; // 最初は非表示
    }

    m_totalNumber = m_canvas->CreateUI<UINumber>();
    m_totalNumber->Initialize("Assets/sprite/", ".DDS", 50.0f * 0.8f, 50.0f * 0.8f, Vector3(Layout::TOTAL_X, Layout::TOTAL_Y, 0.0f), 40.0f);
    m_totalNumber->SetZeroPadding(false);
    m_totalNumber->isDraw = false;
}

GameResultUI::~GameResultUI()
{
    delete m_canvas;
}

void GameResultUI::Update()
{
    // 各行の表示・非表示の制御
    for (int i = 0; i < 5; i++) {
        if (i <= m_activeRowIndex) {
            m_rows[i].scoreNumber->isDraw = true;
        }
    }

    if (m_animPhase >= 1) {
        m_totalNumber->isDraw = true;
    }

    m_canvas->Update();
}

void GameResultUI::Render(RenderContext& rc)
{
    // キャンバス（背景、枠、UINumber）を描画
    m_canvas->Render(rc);

    // 左側の特殊な文字列と右側のランクは手動描画
    for (int i = 0; i < 5; i++) {
        if (i > m_activeRowIndex) continue;

        for (int j = 0; j < 8; j++) {
            if (m_rows[i].valCache[j] != 0) m_rows[i].valSprites[j].Draw(rc);
        }
        if (m_rows[i].rankCache != 0) {
            m_rows[i].rankSprite.Draw(rc);
        }
    }
}

// --- 外部（GameResult）から値を受け取る関数群 ---

void GameResultUI::SetAnimPhase(int activeRowIndex, int animPhase)
{
    m_activeRowIndex = activeRowIndex;
    m_animPhase = animPhase;
}

void GameResultUI::SetRowValueString(int row, const std::string& text)
{
    float y = Layout::ROW_START_Y + (Layout::ROW_STEP_Y * row);
    RefreshStringSprites(m_rows[row].valSprites, m_rows[row].valCache, 8, text, Vector3(Layout::COL_VALUE_X, y, 0.0f), 0.5f);
}

void GameResultUI::SetRowScore(int row, int score)
{
    m_rows[row].scoreNumber->SetNumber(score);
}

void GameResultUI::SetRowRank(int row, char rank)
{
    float y = Layout::ROW_START_Y + (Layout::ROW_STEP_Y * row);
    RefreshRankSprite(m_rows[row].rankSprite, m_rows[row].rankCache, rank, Vector3(Layout::COL_RANK_X, y, 0.0f), 0.5f);
}

void GameResultUI::SetTotalScore(int totalScore)
{
    m_totalNumber->SetNumber(totalScore);
}

// --- 内部関数（元のGameResultから引っ越し） ---

void GameResultUI::RefreshStringSprites(SpriteRender* sprites, char* cache, int maxCount, const std::string& text, const Vector3& pos, float scale)
{
    float numWidth = 60.0f * scale;
    float dotWidth = 15.0f * scale;
    int len = (int)text.length();
    std::vector<float> xCoords(len);

    if (len > 0) {
        xCoords[len - 1] = pos.x;
        for (int i = len - 2; i >= 0; i--) {
            char cRight = text[i + 1];
            char cCurrent = text[i];
            float wRight = (cRight == '.' || cRight == ':') ? dotWidth : numWidth;
            float wCurrent = (cCurrent == '.' || cCurrent == ':') ? dotWidth : numWidth;
            float step = (wRight + wCurrent) / 2.0f;
            xCoords[i] = xCoords[i + 1] - step;
        }
    }

    for (int i = 0; i < maxCount; i++) {
        if (i >= text.length()) {
            cache[i] = 0; continue;
        }
        char c = text[i];
        if (cache[i] != c) {
            cache[i] = c;
            if (c >= '0' && c <= '9') sprites[i].Init(Layout::NUM_TEX[c - '0'], 50.0f, 50.0f);
            else if (c == '.' || c == ':') sprites[i].Init(Layout::DOT_TEX, 40.0f, 40.0f);
            else cache[i] = 0;
        }
        if (cache[i] != 0) {
            Vector3 dPos = pos;
            dPos.x = xCoords[i];
            if (c == '.') dPos.y -= 15.0f * scale;
            sprites[i].SetPosition(dPos);
            sprites[i].SetScale(Vector3(scale, scale, 1.0f));
            sprites[i].Update();
        }
    }
}

void GameResultUI::RefreshRankSprite(SpriteRender& sprite, char& cache, char rank, const Vector3& pos, float scale)
{
    if (cache != rank) {
        cache = rank;
        int idx = 4; // D
        if (rank == 'S') idx = 0;
        else if (rank == 'A') idx = 1;
        else if (rank == 'B') idx = 2;
        else if (rank == 'C') idx = 3;
        sprite.Init(Layout::RANK_TEX[idx], 80.0f, 80.0f);
    }
    sprite.SetPosition(pos);
    sprite.Update();
}