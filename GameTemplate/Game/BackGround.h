#pragma once

/**
 * @brief 背景オブジェクトクラス
 * @details ステージ背景のモデル、物理オブジェクト、回転情報を管理します。
 */
class BackGround : public IGameObject
{
public:
    
    BackGround();
    ~BackGround();


    void Update();

    void Render(RenderContext& rc);

    void Init();

    /** @brief 背景の物理静的オブジェクト */
    PhysicsStaticObject physicsStaticObject;

private:
    /** @brief 背景モデルのレンダラー */
    ModelRender m_map;
    ModelRender m_hitBox;
    ModelRender m_mapAthletic;
    ModelRender m_anim;


    float m_openAngle = 0.0f;


    //初期の位置を記憶しておく変数
    Vector3 m_initialPos = Vector3::Zero;

    //モデルの中心から「ヒンジ（蝶番）」までの距離
    Vector3 m_hingeOffset = Vector3::Zero;
};
