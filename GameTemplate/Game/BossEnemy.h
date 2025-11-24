#pragma once

class BossEnemy : public IGameObject
{
public:
    BossEnemy();
    ~BossEnemy();

    bool Start();
    void Update();
    void Render(RenderContext& rc);

    // EnemyManager から呼ばれる
    void SetBossPosition(const Vector3& pos) { m_initPos = pos; }
    Vector3 GetCorePosition() const { return m_bossPos; }
    CollisionObject* GetCoreCollider() { return m_coreCollision; }

    void SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ);

    // 倒されたときに EnemyManager が呼ぶ
    void OnDefeated();

private:
    void Move();       // 上下移動処理
    void Rotation();   // 見た目の回転

private:
    // モデル
    ModelRender m_bossEnemy;   // 本体
    ModelRender m_body;        // 体（上下に動く床的なもの）

    // ★ 追加: 動く土台の物理判定 (MovingFloorの実装を参考)
    PhysicsStaticObject m_bodyPhysicsStaticObject;

    // 位置
    Vector3 m_initPos;     // StageManager が渡す初期座標
    Vector3 m_bodyPos;     // 体の位置
    Vector3 m_bossPos;     // 本体の位置（体の上に乗る）

    // 回転
    Quaternion m_rot = Quaternion::Identity;

    // 当たり判定（本体の中心当たり判定）
    CollisionObject* m_coreCollision = nullptr;

    // 上下移動
    float m_moveSpeed = 100.0f;
    float m_minY = -120.0f;
    float m_maxY = 80.0f;
    bool  m_movingUp = true;

    // 状態フラグ
    bool m_isAlive = true;
};