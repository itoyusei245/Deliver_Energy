/**
 * @file GameClear.h
 * @brief ゲームクリア画面のクラス定義
 */
#pragma once

 /**
  * @class GameClear
  * @brief ゲームクリア画面クラス
  * @details ゲームクリア時の画像をスプライト表示し、入力待ち（Aボタン）を行ってタイトル画面へ遷移させます。
  */
class GameClear : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details ゲームクリア画面のスプライト初期化を行います。
     */
    GameClear();

    /**
     * @brief デストラクタ
     */
    ~GameClear();

    /**
     * @brief 更新処理
     * @details Aボタンの入力を監視し、押されたらタイトル画面(Title)を生成して自身を破棄します。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param[in,out] rc 描画コンテキスト
     * @details ゲームクリア画像の描画を実行します。
     */
    void Render(RenderContext& rc);

private:
    /** @brief ゲームクリア画面用スプライトレンダラー */
    SpriteRender spriteRender;
};