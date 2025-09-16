#pragma once

/**
 * @brief タイトル画面クラス
 * @details タイトル画面のスプライト表示・遷移処理を管理します。
 */
class Title : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details タイトル画面の初期化を行います。
     */
    Title();

    /**
     * @brief デストラクタ
     * @details タイトル画面の終了処理を行います。
     */
    ~Title();

    /**
     * @brief 更新処理
     * @details 入力判定や画面遷移などの毎フレーム処理を行います。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @details タイトル画面のスプライト描画を行います。
     */
    void Render(RenderContext& rc);

    /** @brief タイトル画面用スプライトレンダラー */
    SpriteRender spriteRender;
};
