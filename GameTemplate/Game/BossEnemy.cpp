#include "stdafx.h"
#include "BossEnemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "GetItem.h"
#include <ctime>
#include <cstdlib>

BossEnemy::BossEnemy()
{
}

BossEnemy::~BossEnemy()
{
    if (m_bodyCollision)
    {
        DeleteGO(m_bodyCollision);
        m_bodyCollision = nullptr;
    }
    if (m_coreCollision)
    {
        DeleteGO(m_coreCollision);
        m_coreCollision = nullptr;
    }
}

/** 初期化（上下移動前の状態に戻す） */
bool BossEnemy::Start()
{
    //---------------------------------------------
    // 体（body）モデル
    //---------------------------------------------
    m_body.Init("Assets/animData/main_bossEnemy.tkm");
    m_body.SetScale(Vector3(80.0f, 80.0f, 80.0f));
    m_body.Update();

    //---------------------------------------------
    // 本体（core）モデル
    //---------------------------------------------
    m_bossEnemy.Init("Assets/animData/main_bossEnemy.tkm");
    m_bossEnemy.SetScale(Vector3(10.0f, 10.0f, 10.0f));
    m_bossEnemy.Update();

    //---------------------------------------------
    // 体と本体の初期位置
    //---------------------------------------------
    m_bodyPos = m_spawnPos;               // ステージ側から SetBossPosition で受け取る
    m_corePos = m_bodyPos + Vector3(0, 120, 0);

    //---------------------------------------------
    // 衝突判定（本体）
    //---------------------------------------------
    float radius = 25.0f;
    float height = 50.0f;

    m_coreCollision = NewGO<CollisionObject>(0, "bossCoreCollision");
    m_coreCollision->CreateBox(m_corePos, Quaternion::Identity, Vector3(radius, height, radius));
    m_coreCollision->SetIsEnableAutoDelete(false);

    //---------------------------------------------
    // 衝突判定（体）
    //---------------------------------------------
    m_bodyCollision = NewGO<CollisionObject>(0, "bossBodyCollision");
    m_bodyCollision->CreateBox(
        m_bodyPos,
        Quaternion::Identity,
        Vector3(200.0f, 200.0f, 200.0f)  // 体のサイズに合わせて調整
    );
    m_bodyCollision->SetIsEnableAutoDelete(false);

    return true;
}

/** 更新処理（動きなしの復元版） */
void BossEnemy::Update()
{
    // モデル位置を反映
    m_body.SetPosition(m_bodyPos);
    m_bossEnemy.SetPosition(m_corePos);

    // 衝突判定も位置更新
    if (m_bodyCollision)
        m_bodyCollision->SetPosition(m_bodyPos);
    if (m_coreCollision)
        m_coreCollision->SetPosition(m_corePos);

    // モデル更新
    m_body.Update();
    m_bossEnemy.Update();
}

/** コイン生成（変更なし） */
void BossEnemy::SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ)
{
    for (int i = 0; i < count; ++i)
    {
        float offsetX = ((float)rand() / RAND_MAX) * 2.0f * rangeX - rangeX;
        float offsetZ = ((float)rand() / RAND_MAX) * 2.0f * rangeZ - rangeZ;

        Vector3 pos = center;
        pos.x += offsetX;
        pos.z += offsetZ;
        pos.y += 10.0f;

        GetItem* coin = NewGO<GetItem>(0, "coin");
        coin->Init(pos);
    }
}

void BossEnemy::Render(RenderContext& rc)
{
    m_body.Draw(rc);
    m_bossEnemy.Draw(rc);
}

///** 呼び出された側で初期位置を指定するための関数 */
//void BossEnemy::SetBossPosition(const Vector3& pos)
//{
//    m_spawnPos = pos;
//}
