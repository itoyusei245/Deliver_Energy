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

    void Render(RenderContext& rc);

    void Init();

    /** @brief 背景の物理静的オブジェクト */
    PhysicsStaticObject physicsStaticObject;

private:
    /** @brief 背景モデルのレンダラー */
    ModelRender m_map;
    ModelRender m_hitBox;
    ModelRender m_mapAthletic;



};
