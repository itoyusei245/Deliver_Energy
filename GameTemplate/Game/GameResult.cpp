#include "stdafx.h"
#include "GameResult.h"
#include "Game.h"
#include "Title.h"
#include "Sound/SoundManager.h"
#include <iomanip> // string stream用
#include <sstream>

// レイアウト定数
namespace Layout {
    const float ROW_START_Y =  190.0f;
    const float ROW_STEP_Y  =  -83.0f;
    const float COL_VALUE_X =  400.0f;
    const float COL_SCORE_X =  600.0f;
    const float COL_RANK_X  =  675.0f;
    const float TOTAL_X     =  695.0f;
    const float TOTAL_Y     = -255.0f;
}

GameResult::GameResult()
{
    m_bg.Init("Assets/sprite/result/Result.DDS", 1920.0f, 1080.0f);
    m_scoreFrame.Init("Assets/sprite/result/ScoreDisplay_W.DDS", 1920.0f, 1080.0f);

    SoundManager::Get().PlayBGM(enSoundKind_Result);

    CalculateScore();
}

GameResult::~GameResult()
{
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
    // --- カウントアップ演出 ---
    if (m_animPhase == 0) {
        
        //現在処理する項目を取得
        ScoreItem& item = m_items[m_activeItemIndex];


        //目標スコアに達していない場合、加算する
        if (item.currentDisp < item.score) {
            //残りの差分に応じて加算量を調整
            int step = max(3333, (item.score - item.currentDisp) / 10);

            item.currentDisp += step;


            //行きすぎたら合わせる
            if (item.currentDisp >= item.score) {
               
                item.currentDisp = item.score;
            }
        }
        else {
            //カウントアップが終わったら、次の項目へ
            m_activeItemIndex++;


            //最後の項目まで達すると合計スコアへ
            if (m_activeItemIndex >= 5) {
                //SE

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
            NewGO<Game>(0, "game");
            DeleteGO(this);
        }
        else if (g_pad[0]->IsTrigger(enButtonB)) {
            SoundManager::Get().PlaySE(enSoundKind_Decision);
            NewGO<Title>(0, "title");
            DeleteGO(this);
        }
    }

    // スキップ
    if (m_animPhase < 2 && g_pad[0]->IsTrigger(enButtonA)) {
        for (int i = 0; i < 5; i++) m_items[i].currentDisp = m_items[i].score;
        m_totalScoreDisp = m_totalScore;
        m_activeItemIndex = 5;
        m_animPhase = 2;
    }

    m_bg.Update();
    m_scoreFrame.Update();

    // ==========================================
    // スプライトの更新処理
    // ==========================================
    for (int i = 0; i < 5; i++)
    {
        //順番が来ていない項目は処理しない
        if (i > m_activeItemIndex)continue;


        float y = Layout::ROW_START_Y + (Layout::ROW_STEP_Y * i);

        // 1. 左側の値 (文字列を作成)
        std::stringstream ssVal;
        if (m_items[i].type == ScoreItem::TYPE_TIME) {
            // 00:00.00 形式
            int totalSec = (int)m_items[i].floatVal;
            int min = totalSec / 60;
            int sec = totalSec % 60;
            int msec = (int)((m_items[i].floatVal - totalSec) * 100);
            ssVal << std::setfill('0') << std::setw(2) << min << ":"
                << std::setfill('0') << std::setw(2) << sec << "."
                << std::setfill('0') << std::setw(2) << msec;
        }
        else if (m_items[i].type == ScoreItem::TYPE_FLOAT) {
            // 13.4 形式
            ssVal << std::fixed << std::setprecision(1) << m_items[i].floatVal;
        }
        else {
            // 整数
            ssVal << m_items[i].intVal;
        }
        // スプライト更新
        RefreshStringSprites(m_rows[i].valSprites, m_rows[i].valCache, 8, ssVal.str(), Vector3(Layout::COL_VALUE_X, y, 0.0f), 0.5f);


        // 2. 真ん中のスコア
        std::string scoreStr = std::to_string(m_items[i].currentDisp);
        RefreshStringSprites(m_rows[i].scoreSprites, m_rows[i].scoreCache, 8, scoreStr, Vector3(Layout::COL_SCORE_X, y, 0.0f), 0.4f);


        // 3. 右のランク (カウントアップ終了後のみ)
        if (m_items[i].currentDisp == m_items[i].score) {
            RefreshRankSprite(m_rows[i].rankSprite, m_rows[i].rankCache, m_items[i].rank, Vector3(Layout::COL_RANK_X, y, 0.0f),0.5f);
        }
    }

    // 4. 合計スコア
    if (m_animPhase >= 1) {
        std::string totalStr = std::to_string(m_totalScoreDisp);
        RefreshStringSprites(m_totalSprites, m_totalCache, 10, totalStr, Vector3(Layout::TOTAL_X, Layout::TOTAL_Y, 0.0f), 0.8f);
    }
}

void GameResult::Render(RenderContext& rc)
{
    m_bg.Draw(rc);
    m_scoreFrame.Draw(rc);

    // 各行の描画
    for (int i = 0; i < 5; i++) {
        // 順番が来ていないスコアは表示しない
        if (i > m_activeItemIndex)continue;
        // 値
        for (int j = 0; j < 8; j++) {
            if (m_rows[i].valCache[j] != 0) m_rows[i].valSprites[j].Draw(rc);
        }
        // スコア
        for (int j = 0; j < 8; j++) {
            if (m_rows[i].scoreCache[j] != 0) m_rows[i].scoreSprites[j].Draw(rc);
        }
        // ランク
        if (m_rows[i].rankCache != 0) {
            m_rows[i].rankSprite.Draw(rc);
        }
    }

    // 合計
    if (m_animPhase >= 1) {
        for (int j = 0; j < 10; j++) {
            if (m_totalCache[j] != 0) m_totalSprites[j].Draw(rc);
        }
    }
}

// 文字列を受け取ってスプライト配列に反映する関数
void GameResult::RefreshStringSprites(SpriteRender* sprites, char* cache, int maxCount, std::string text, const Vector3& pos, float scale)
{
    float numWidth = 60.0f * scale; // 文字幅
    float dotWidth = 15.0f * scale;


    int len = (int)text.length();

    // 1. 各文字のX座標を計算（右端基準で左へ積み上げ）
    // 配列にそれぞれの座標を記憶させておきます
    std::vector<float> xCoords(len);

    if (len > 0) {
        // 一番右の文字は、指定された pos.x に置く
        xCoords[len - 1] = pos.x;

        // 右から左へ向かって、隣との間隔を計算しながら座標を決める
        for (int i = len - 2; i >= 0; i--) {
            char cRight = text[i + 1];   // 右隣の文字
            char cCurrent = text[i];     // 今の文字

            // それぞれの文字の「幅」を判定
            float wRight = (cRight == '.' || cRight == ':') ? dotWidth : numWidth;
            float wCurrent = (cCurrent == '.' || cCurrent == ':') ? dotWidth : numWidth;

            // 間隔 = (右の幅半分 + 今の幅半分) 
            // こうすることで、文字の中心同士の距離が適切になります
            float step = (wRight + wCurrent) / 2.0f;

            // 左隣の座標 = 右の座標 - 間隔
            xCoords[i] = xCoords[i + 1] - step;
        }
    }

    for (int i = 0; i < maxCount; i++)
    {
        // 文字列の範囲外なら非表示(キャッシュを0に)
        if (i >= text.length()) {
            cache[i] = 0;
            continue;
        }

        char c = text[i];

        // 前回と同じ文字ならInitスキップ、違えばロードしなおす
        if (cache[i] != c) {
            cache[i] = c;

            if (c >= '0' && c <= '9') {
                sprites[i].Init(NUM_TEX[c - '0'], 50.0f, 50.0f);
            }
            else if (c == '.' || c == ':') {
                // ドットもコロンもdot.DDSで代用
                sprites[i].Init(DOT_TEX, 40.0f, 40.0f);
            }
            else {
                // 不明な文字は非表示扱い
                cache[i] = 0;
            }
            // Initするとピボットがリセットされる場合があるので再設定
            // sprites[i].SetPivot({0.0f, 0.5f}); 
        }

        // 表示位置とスケールの更新
        if (cache[i] != 0) {
            Vector3 dPos = pos;
            dPos.x = xCoords[i]; // 計算済みのX座標を使う

            // ドットのY座標微調整
            if (c == '.') dPos.y -= 15.0f * scale;

            sprites[i].SetPosition(dPos);
            sprites[i].SetScale(Vector3(scale, scale, 1.0f));
            sprites[i].Update();
        }
    }
}

// ランク用ヘルパー
void GameResult::RefreshRankSprite(SpriteRender& sprite, char& cache, char rank, const Vector3& pos , float scale)
{
    if (cache != rank) {
        cache = rank;
        int idx = 4; // D
        if (rank == 'S') idx = 0;
        else if (rank == 'A') idx = 1;
        else if (rank == 'B') idx = 2;
        else if (rank == 'C') idx = 3;

        sprite.Init(RANK_TEX[idx], 80.0f, 80.0f);
    }
    sprite.SetPosition(pos);
    sprite.Update();
}