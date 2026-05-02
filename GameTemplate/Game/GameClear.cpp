/**
 * @file GameClear.cpp
 * @brief ゲームクリアオブジェクトの実装
 */
#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Player.h"
#include "GameResult.h"
#include "UI/GameTimeUI.h" 

namespace {
    constexpr const char* PATH_GOAL_MODEL = "Assets/modelData/Stage/Goal.tkm";

    const Vector3 SCALE_GOAL(1.0f, 1.0f, 1.0f);
    const Vector3 POS(0.0f, 0.0f, 0.0f);

    constexpr float GOAL_HIT_RADIUS = 50.0f;

    constexpr float HP_PERCENT_RATE = 100.0f;
}

GameClear::GameClear()
{
    m_position = POS;

    // ゴールのモデル読み込みと初期化
    m_modelRender.Init(PATH_GOAL_MODEL);
    m_modelRender.SetScale(SCALE_GOAL);
    m_modelRender.SetPosition(m_position);
}

GameClear::~GameClear()
{
}

void GameClear::Update()
{
    if (m_isGoal) return;

    // プレイヤーとの距離判定
    Player* player = FindGO<Player>("player");
    if (player != nullptr)
    {
        Vector3 diff = player->GetPosition() - m_position;

        if (diff.Length() < GOAL_HIT_RADIUS)
        {
            m_isGoal = true;
            Game::IsGamePlay = false; 

            // --- リザルトへデータを渡す ---
            // 定数を使ってHPのパーセンテージを計算
            Game::FinalHP = player->GetHPRate() * HP_PERCENT_RATE;

            // リザルト画面へ遷移
            // 現在のGameシーンを削除
            DeleteGO(FindGO<Game>("game"));

            // リザルトシーン生成
            NewGO<GameResult>(0, "gameResult");
        }
    }

    m_modelRender.Update();
}

void GameClear::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);
}

void GameClear::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_modelRender.SetPosition(pos);
}