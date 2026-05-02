/**
 * @file GameLoading.cpp
 * @brief ゲームロード画面の実装
 */
#include "stdafx.h"
#include "GameLoading.h"
#include "UI/LoadUI.h"
#include "Game.h" 

namespace {
    // --- オブジェクト検索・生成用の名前 ---
    constexpr const char* NAME_LOAD_UI = "loadingUI";
    constexpr const char* NAME_GAME = "game";

    // --- ロード制御 ---
    // ロード開始までの待機フレーム数
    // ※生成後すぐに重い処理(Gameの生成)を始めると、ロード画像が描画される前に
    // フリーズしたように見えてしまうため、数フレーム待機させます。
    constexpr int WAIT_FRAMES_BEFORE_LOAD = 10;
}

GameLoading::GameLoading()
{
}

GameLoading::~GameLoading()
{
    LoadUI* ui = FindGO<LoadUI>(NAME_LOAD_UI);
    if (ui != nullptr) {
        DeleteGO(ui);
    }
}

bool GameLoading::Start()
{
    // 定数を使ってUIを生成
    NewGO<LoadUI>(0, NAME_LOAD_UI);
    return true;
}

void GameLoading::Update()
{
    m_timer++;

    if (m_timer >= WAIT_FRAMES_BEFORE_LOAD) {

        NewGO<Game>(0, NAME_GAME);

        // 自身を削除（デストラクタが呼ばれ、UIも一緒に消えます）
        DeleteGO(this);
    }
}