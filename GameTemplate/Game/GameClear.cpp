#include "stdafx.h"
#include "GameClear.h"
#include"Title.h"

GameClear::GameClear()
{
    /**ゲームクリアの画像を読み込み*/
	spriteRender.Init("Assets/sprite/Clear.dds", 1920.0f, 1080.0f);
}

GameClear::~GameClear()
{

}

/**
 * @brief 毎フレームの更新処理
 * @details Aボタンが押されたらタイトル画面へ遷移し、自身を削除します。
 */
void GameClear::Update()
{
    /** Aボタンが押されたら…*/
    if (g_pad[0]->IsTrigger(enButtonA))
    {
		/** タイトルのオブジェクトを作成*/
        NewGO<Title>(0, "title");
        /** 自身を削除する*/
        DeleteGO(this);
    }
}

/**
 * @brief ゲームクリア画面の描画処理
 * @param rc 描画コンテキスト
 * @details クリア画像の描画を行います。
 */
void GameClear::Render(RenderContext& rc)
{
    spriteRender.Draw(rc);
}