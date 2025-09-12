#pragma once
class Enemy : public IGameObject
{
public:
    Enemy();
    ~Enemy();

    void Update();
    void Render(RenderContext& rc);

    void Move();
    void Rotation();

    ModelRender          modelRender;
    PhysicsStaticObject  physicsStaticObject;
    Quaternion           m_rot;

private:
    // パトロール用
    std::vector<Vector3> waypoints;   // 巡回ポイント
    int currentWaypoint = 0;          // 現在の目標インデックス
    float speed = 0.8f;               // 移動速度
	Vector3 currentPos;        // 現在位置
};

