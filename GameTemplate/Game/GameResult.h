#pragma once
#include <string>
#include <vector>

class GameResult : public IGameObject
{
public:
    GameResult();
    ~GameResult();
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    // --- スコア計算用データ ---
    struct ScoreItem {
        int score = 0;       // 計算後のスコア
        int currentDisp = 0; // 表示用（カウントアップ演出用）
        char rank = 'D';     // ランク

        // 左側に表示する値の種類と中身
        enum Type { TYPE_FLOAT, TYPE_INT, TYPE_TIME };
        Type type = TYPE_INT;
        float floatVal = 0.0f;
        int intVal = 0;
    };
    ScoreItem m_items[5]; // 0:Temp, 1:Enemy, 2:HP, 3:Coin, 4:Time

    int m_totalScore = 0;
    int m_totalScoreDisp = 0;

    // --- 演出管理 ---
    int m_animPhase = 0;

    // --- カウントアップ中の項目インデックス　---
    int m_activeItemIndex = 0;

    // --- 画像リソース ---
    SpriteRender m_bg;         // 背景
    SpriteRender m_scoreFrame; // 枠

    // 画像パス配列
    const char* NUM_TEX[10] = {
        "Assets/sprite/0.DDS", "Assets/sprite/1.DDS", "Assets/sprite/2.DDS", "Assets/sprite/3.DDS", "Assets/sprite/4.DDS",
        "Assets/sprite/5.DDS", "Assets/sprite/6.DDS", "Assets/sprite/7.DDS", "Assets/sprite/8.DDS", "Assets/sprite/9.DDS"
    };
    const char* DOT_TEX = "Assets/sprite/dot.DDS";
    const char* RANK_TEX[5] = {
        "Assets/sprite/result/S.DDS", "Assets/sprite/result/A.DDS", "Assets/sprite/result/B.DDS", "Assets/sprite/result/C.DDS", "Assets/sprite/result/D.DDS"
    };

    // --- 表示用スプライト管理構造体 ---
    // 1行分の表示パーツをまとめたもの
    struct RowDisplay {
        // 左側の値（最大8桁分用意しておく）
        SpriteRender valSprites[8];
        char valCache[8]; // 現在セットされている文字を覚える（無駄なInit防止）

        // 真ん中のスコア（最大8桁）
        SpriteRender scoreSprites[8];
        char scoreCache[8];

        // 右側のランク
        SpriteRender rankSprite;
        char rankCache = 0;

        // コンストラクタでキャッシュを初期化
        RowDisplay() {
            for (int i = 0; i < 8; i++) { valCache[i] = 0; scoreCache[i] = 0; }
        }
    };

    RowDisplay m_rows[5]; // 5行分

    // 合計スコア用（最大10桁）
    SpriteRender m_totalSprites[10];
    char m_totalCache[10] = { 0 };

    // --- 関数 ---
    void CalculateScore();

    // スプライトの絵を更新して配置するヘルパー関数
    void RefreshStringSprites(SpriteRender* sprites, char* cache, int maxCount, std::string text, const Vector3& pos, float scale);
    void RefreshRankSprite(SpriteRender& sprite, char& cache, char rank, const Vector3& pos,float scale);
};