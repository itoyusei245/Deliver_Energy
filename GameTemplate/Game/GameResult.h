#pragma once
#include <string>
#include <vector>
#include "UI/GameResultUI.h"

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
        int score = 0;
        int currentDisp = 0;
        char rank = 'D';

        enum Type { TYPE_FLOAT, TYPE_INT, TYPE_TIME };
        Type type = TYPE_INT;
        float floatVal = 0.0f;
        int intVal = 0;
    };
    ScoreItem m_items[5]; 

    int m_totalScore = 0;
    int m_totalScoreDisp = 0;

    // --- 演出管理 ---
    int m_animPhase = 0;
    int m_activeItemIndex = 0;

    // --- 新しいUIシステム ---
    GameResultUI* m_ui = nullptr;

    // --- 関数 ---
    void CalculateScore();
};