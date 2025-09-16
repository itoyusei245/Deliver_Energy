#include "stdafx.h"
#include "Title.h"
#include "Game.h"

/**
 * @brief Titleクラスのコンストラクタ
 * @details タイトル画面用のスプライト画像を初期化します。
 */
Title::Title()
{
    //! タイトル画面の画像を読み込み
    spriteRender.Init("Assets/sprite/Title.DDS", 1920.0f, 1080.0f);
}

/**
 * @brief Titleクラスのデストラクタ
 */
Title::~Title()
{
	
}

/**
 * @brief 毎フレームの更新処理
 * @details Aボタンが押されたらゲーム画面へ遷移し、自身を削除します。
 */
void Title::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //! ゲームのオブジェクトを作成
        NewGO<Game>(0,"game");
        //! 自身を削除する
        DeleteGO(this);
    }
}

/**
 * @brief タイトル画面の描画処理
 * @param rc 描画コンテキスト
 * @details タイトル画像の描画を行います。
 */
void Title::Render(RenderContext& rc)
{
    spriteRender.Draw(rc);
}

