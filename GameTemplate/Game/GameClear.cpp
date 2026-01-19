/**
 * @file GameClear.cpp
 * @brief ゲームクリア画面の実装
 */
#include "stdafx.h"
#include "GameClear.h"
#include "Title.h" // タイトル画面への遷移用

 /**
  * @brief コンストラクタ
  * @details "Assets/sprite/Clear.dds" を読み込み、画面サイズ(1920x1080)で初期化します。
  */
GameClear::GameClear()
{
    /** ゲームクリアの画像を読み込み */
    spriteRender.Init("Assets/sprite/Clear.dds", 1920.0f, 1080.0f);
}

GameClear::~GameClear()
{
}

/**
 * @brief 更新処理
 * @details
 * コントローラーのAボタン入力を検知すると、以下の処理を行います。
 * 1. Title（タイトル画面）オブジェクトの生成
 * 2. 自身（GameClear）の削除
 */
void GameClear::Update()
{
    /** Aボタンが押されたら… */
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        /** タイトルのオブジェクトを作成 */
        NewGO<Title>(0, "title");

        /** 自身を削除する（シーン遷移） */
        DeleteGO(this);
    }
}

/**
 * @brief 描画処理
 */
void GameClear::Render(RenderContext& rc)
{
    spriteRender.Draw(rc);
}