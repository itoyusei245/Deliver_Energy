#include "stdafx.h"
#include "BossEnemy.h"
#include "Player.h"
#include "GetItem.h"

BossEnemy::BossEnemy() {}
BossEnemy::~BossEnemy()
{
    if (m_coreCollision)
    {
        DeleteGO(m_coreCollision);
        m_coreCollision = nullptr;
    }
    // PhysicsStaticObject は自動解放されるので記述不要
}

bool BossEnemy::Start()
{
    // === 1. 土台のモデルを「原点」で初期化する ===
    m_body.Init("Assets/animData/main_bossEnemy.tkm");
    m_body.SetScale(Vector3(70.0f, 70.0f, 70.0f));
    // ★重要: 一度位置をゼロにして更新。
    // これにより、物理判定が「原点」を中心に作られるようになります。
    m_body.SetPosition(Vector3::Zero);
    m_body.Update();

    // === 2. 物理判定の生成 ===
    // 原点にあるモデル情報を使って作成（行列操作は不要）
    m_bodyPhysicsStaticObject.CreateFromModel(m_body.GetModel(), m_body.GetModel().GetWorldMatrix());

    // === 3. 本来の位置へ移動させる ===
    // SetBossPosition()で受け取った初期位置を使用
    m_bodyPos = m_initPos;
    // 指定通り本体位置を +1000.0f に変更
    m_bossPos = m_bodyPos + Vector3(0.0f, 1000.0f, 0.0f);

    // モデルと物理判定を正しい位置へセット
    m_body.SetPosition(m_bodyPos);
    m_body.Update();
    m_bodyPhysicsStaticObject.SetPosition(m_bodyPos);

    // === 4. 本体（コア）の初期化 ===
    m_bossEnemy.Init("Assets/animData/main_bossEnemy.tkm");
    m_bossEnemy.SetScale(Vector3(10.0f, 10.0f, 10.0f));
    m_bossEnemy.SetPosition(m_bossPos);
    m_bossEnemy.Update();

    // === 5. 当たり判定（コア用） ===
    m_coreCollision = NewGO<CollisionObject>(0, "BossCoreCollider");
    Vector3 size = { 40.0f, 60.0f, 40.0f };
    m_coreCollision->CreateBox(m_bossPos, Quaternion::Identity, size);
    m_coreCollision->SetIsEnableAutoDelete(false);

    return true;
}

void BossEnemy::Update()
{
    if (!m_isAlive) return;

    Move();
    Rotation();

    // モデル更新
    m_body.Update();
    m_bossEnemy.Update();

    // ★ 土台の物理判定をモデルの位置に同期
    m_bodyPhysicsStaticObject.SetPosition(m_bodyPos);

    // コアの当たり判定を同期
    if (m_coreCollision)
    {
        m_coreCollision->SetPosition(m_bossPos);
        m_coreCollision->SetRotation(m_rot);
    }
}

void BossEnemy::Move()
{
    float dt = g_gameTime->GetFrameDeltaTime();

    // 上下移動ロジック
    if (m_movingUp)
    {
        m_bodyPos.y += m_moveSpeed * dt;
        if (m_bodyPos.y >= m_maxY)
        {
            m_bodyPos.y = m_maxY;
            m_movingUp = false;
        }
    }
    else
    {
        m_bodyPos.y -= m_moveSpeed * dt;
        if (m_bodyPos.y <= m_minY)
        {
            m_bodyPos.y = m_minY;
            m_movingUp = true;
        }
    }

    // 本体は土台の +1000.0f 上
    m_bossPos = m_bodyPos + Vector3(0.0f, 800.0f, 0.0f);

    m_body.SetPosition(m_bodyPos);
    m_bossEnemy.SetPosition(m_bossPos);
}

void BossEnemy::Rotation()
{
    // 回転演出
    Quaternion q;
    q.SetRotationDegY(1.0f);
    m_rot.Multiply(m_rot, q);
    m_bossEnemy.SetRotation(m_rot);
}

void BossEnemy::OnDefeated()
{
    if (!m_isAlive) return;

    m_isAlive = false;

    if (m_coreCollision)
    {
        DeleteGO(m_coreCollision);
        m_coreCollision = nullptr;
    }

    // 倒されたら土台の物理判定もなくす
    m_bodyPhysicsStaticObject.Release();
}

void BossEnemy::SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ)
{
    for (int i = 0; i < count; i++)
    {
        float ox = ((float)rand() / RAND_MAX) * 2 * rangeX - rangeX;
        float oz = ((float)rand() / RAND_MAX) * 2 * rangeZ - rangeZ;

        Vector3 pos = center;
        pos.x += ox;
        pos.z += oz;
        pos.y += 10.0f;

        auto* coin = NewGO<GetItem>(0, "coin");
        coin->Init(pos);
    }
}

void BossEnemy::Render(RenderContext& rc)
{
    if (!m_isAlive) return;

    m_body.Draw(rc);
    m_bossEnemy.Draw(rc);
}