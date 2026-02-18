#pragma once
class GameClear : public IGameObject
{
public:
    GameClear();
    ~GameClear();
    void Update() override;
    void Render(RenderContext& rc) override;

    // 座標セット用
    void SetPosition(const Vector3& pos) {
        m_position = pos;
        m_modelRender.SetPosition(pos);
    }

private:
    ModelRender m_modelRender;
    Vector3 m_position = Vector3::Zero;
    bool m_isGoal = false; // 重複判定防止
};