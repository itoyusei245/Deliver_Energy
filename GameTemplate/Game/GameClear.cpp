#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Player.h"
#include "GameResult.h"
#include "GameTimeUI.h" 

GameClear::GameClear()
{
    // ゴールのモデル読み込み
    m_modelRender.Init("Assets/modelData/Stage/Goal.tkm");
    m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));
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
        if (diff.Length() < 50.0f) // 半径50以内ならゴールとみなす
        {
            m_isGoal = true;
            Game::IsGamePlay = false; // ゲーム停止

            // --- リザルトへデータを渡す ---

            Game::FinalHP = player->GetHPRate() * 100.0f;

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