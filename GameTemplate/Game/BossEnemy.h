#pragma once

class BossEnemy : public IGameObject
{
public:
    BossEnemy();
    ~BossEnemy();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    void SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ);

    void SetBossPosition(const Vector3& pos)
    {
        m_spawnPos = pos;
    }

    CollisionObject* GetCoreCollider() const { return m_coreCollision; }
    const Vector3& GetCorePosition() const { return m_corePos; }

private:
    // モデル
    ModelRender m_body;
    ModelRender m_bossEnemy;

    // 衝突判定
    CollisionObject* m_bodyCollision = nullptr;
    CollisionObject* m_coreCollision = nullptr;

    // 位置
    Vector3 m_spawnPos = Vector3::Zero;  // スポーン位置
    Vector3 m_bodyPos = Vector3::Zero;   // 体の位置
    Vector3 m_corePos = Vector3::Zero;   // 本体の位置
};
