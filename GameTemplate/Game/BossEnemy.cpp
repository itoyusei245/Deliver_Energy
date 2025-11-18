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
}

bool BossEnemy::Start()
{
    // ★ 呼び出し側（StageManager）が SetBossPosition() してくれる前提！
    //   m_initPos が初期値になる

    // === モデル読み込み ==========================
    m_body.Init("Assets/animData/main_bossEnemy.tkm");
    m_body.SetScale(Vector3(80.0f, 80.0f, 80.0f));
    m_body.Update();

    m_bossEnemy.Init("Assets/animData/main_bossEnemy.tkm");
    m_bossEnemy.SetScale(Vector3(10.0f, 10.0f, 10.0f));
    m_bossEnemy.Update();

    // === 体と本体の初期位置 ==========================
    m_bodyPos = m_initPos;
    m_bossPos = m_bodyPos + Vector3(0.0f, 120.0f, 0.0f);

    // === 当たり判定（本体用） ==========================
    m_coreCollision = NewGO<CollisionObject>(0, "BossCoreCollider");
    Vector3 size = { 40.0f, 60.0f, 40.0f };  // コア当たりサイズ
    m_coreCollision->CreateBox(m_bossPos, Quaternion::Identity, size);
    m_coreCollision->SetIsEnableAutoDelete(false);

    return true;
}

void BossEnemy::Update()
{
    if (!m_isAlive) return;

    Move();
    Rotation();

    // Update
    m_body.Update();
    m_bossEnemy.Update();

    // 当たり判定追従
    if (m_coreCollision)
    {
        m_coreCollision->SetPosition(m_bossPos);
        m_coreCollision->SetRotation(m_rot);
    }
}

void BossEnemy::Move()
{
    float dt = g_gameTime->GetFrameDeltaTime();

    // ---- 上下移動 ----
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

    // 本体を体の上に乗せる
    m_bossPos = m_bodyPos + Vector3(0.0f, 120.0f, 0.0f);

    m_body.SetPosition(m_bodyPos);
    m_bossEnemy.SetPosition(m_bossPos);
}

void BossEnemy::Rotation()
{
    // 軽い回転演出
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
