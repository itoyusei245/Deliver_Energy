#pragma once

/**
 * @brief ゲームクリア画面クラス
 * @details ゲームクリア時のスプライト表示・遷移処理を管理します。
 */
class GameClear : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details ゲームクリア画面の初期化を行います。
     */
    GameClear();

    /**
     * @brief デストラクタ
     * @details ゲームクリア画面の終了処理を行います。
     */
    ~GameClear();

    /**
     * @brief 更新処理
     * @details 入力判定や画面遷移などの毎フレーム処理を行います。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @details ゲームクリア画面のスプライト描画を行います。
     */
    void Render(RenderContext& rc);

    /** @brief ゲームクリア画面用スプライトレンダラー */
    SpriteRender spriteRender;
};

