/**
 * @file GameResultUI.h
 * @brief リザルト画面の描画のみを担当するUIクラス
 */
#pragma once
#include "UIBase.h"

class GameResultUI
{
public:
    GameResultUI();
    ~GameResultUI();

    void Update();
    void Render(RenderContext& rc);

   // アニメーションの進行状況（現在何行目をカウントアップ中か、合計スコア画面か）をセット
    void SetAnimPhase(int activeRowIndex, int animPhase);

    // 左側の固定値文字列をセット
    void SetRowValueString(int row, const std::string& text);

    // 真ん中のスコア（カウントアップする数字）をセット
    void SetRowScore(int row, int score);

    // 右側のランク（S, A, B, C, D）をセット
    void SetRowRank(int row, char rank);

    // 一番下の合計スコアをセット
    void SetTotalScore(int totalScore);

private:
    // 文字列からスプライトを生成して並べる関数
    void RefreshStringSprites(SpriteRender* sprites, char* cache, int maxCount, const std::string& text, const Vector3& pos, float scale);
    void RefreshRankSprite(SpriteRender& sprite, char& cache, char rank, const Vector3& pos, float scale);

private:
    UICanvas* m_canvas = nullptr;

    UIIcon* m_bg = nullptr;
    UIIcon* m_scoreFrame = nullptr;

    // --- 各行のデータ ---
    struct RowUI {
        SpriteRender valSprites[8]; 
        char valCache[8] = { 0 };

        UINumber* scoreNumber = nullptr; // 真ん中のスコア

        SpriteRender rankSprite;         // ランク画像
        char rankCache = 0;
    };
    RowUI m_rows[5];

    // 合計スコア
    UINumber* m_totalNumber = nullptr; 

    // 表示制御用
    int m_activeRowIndex = 0;
    int m_animPhase = 0;
};