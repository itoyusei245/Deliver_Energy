#pragma once

/**
 * @brief 背景オブジェクトクラス
 * @details ステージ背景のモデル、物理オブジェクト、回転情報を管理します。
 */
class BackGround : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details 背景オブジェクトの初期化を行います。
     */
    BackGround();

    /**
     * @brief デストラクタ
     * @details 背景オブジェクトの終了処理を行います。
     */
    ~BackGround();

    /**
     * @brief 背景の描画処理
     * @param rc 描画コンテキスト
     * @details 背景モデルの描画を行います。
     */
    void Render(RenderContext& rc);

    /** @brief 背景モデルのレンダラー */
    ModelRender modelRender;

    /** @brief 背景の物理静的オブジェクト */
    PhysicsStaticObject physicsStaticObject;

    /** @brief 背景モデルの回転情報 */
    Quaternion m_rot;
};
