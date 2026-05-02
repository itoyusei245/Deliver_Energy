/**
 * @file GameClear.h
 * @brief ゲームクリア（ゴール）オブジェクトのクラス定義
 */
#pragma once

class GameClear : public IGameObject
{
public:
    GameClear();
    ~GameClear();

    void Update() override;
    void Render(RenderContext& rc) override;

    // ゴールの座標をセットする
    void SetPosition(const Vector3& pos);

private:
    ModelRender m_modelRender;
    Vector3 m_position;
    bool m_isGoal = false; 
};