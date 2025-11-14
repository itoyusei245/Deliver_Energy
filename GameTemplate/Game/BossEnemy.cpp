#include "stdafx.h"
#include "BossEnemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "GetItem.h"
#include <ctime>
#include <cstdlib>

/** コンストラクタ / デストラクタ */
BossEnemy::BossEnemy() {}
BossEnemy::~BossEnemy()
{
   /* if (collision)
    {
        DeleteGO(collision);
        collision = nullptr;
    }*/
}

/** 初期化 */
bool BossEnemy::Start()
{
    // ボス本体モデル
    m_bossEnemy.Init("Assets/animData/main_bossEnemy.tkm");
    m_bossEnemy.SetScale(Vector3(10.0f, 10.0f, 10.0f));
    m_bossEnemy.Update();

    // 動く体モデル（tkm）
    m_body.Init("Assets/animData/main_bossEnemy.tkm");
    m_body.SetScale(Vector3(80.0f, 80.0f, 80.0f));
    m_body.Update();

    // 初期位置（任意でセット）
    m_bodyPos = Vector3(3500.0f, -100.0f, -4200.0f);
    m_bossPos = m_bodyPos + Vector3(0.0f, 120.0f, 0.0f);

     /**キャラコンはボス本体位置で初期化（位置参照用に残す）*/
    float radius = 25.0f;
    float height = 50.0f;
    

    // 衝突オブジェクト（当たり判定）
    collision = NewGO<CollisionObject>(0, "BossCollision");
    Vector3 boxSize(radius, height, radius);
    collision->CreateBox(m_bossPos, m_rot, boxSize);
    collision->SetIsEnableAutoDelete(false);

    // movement params (必要なら外部設定可)
    m_moveSpeed = 100.0f;
    m_minY = -120.0f;
    m_maxY = 80.0f;
    m_movingUp = true;
    m_isAlive = true;

    // 乱数初期化（コイン生成で使用）
    std::srand((unsigned int)std::time(nullptr));

    return true;
}

/** 毎フレーム更新（衝突チェックは行うが削除は行わない） */
void BossEnemy::Update()
{
    if (!m_isAlive) return;

    // 動きと回転
    Move();
    Rotation();

    // モデル更新
    m_body.Update();
    m_bossEnemy.Update();

    // 衝突オブジェクト更新（位置・回転を常に反映）
    if (collision)
    {
        collision->SetPosition(m_bossPos);
        collision->SetRotation(m_rot);
    }

    // ※ プレイヤー判定は EnemyManager に任せる（ここで自己削除やコイン生成は行わない）
}

/** 上下移動（体を動かし、本体を追従させる） */
void BossEnemy::Move()
{
    float deltaTime = g_gameTime->GetFrameDeltaTime();

    if (m_movingUp)
    {
        m_bodyPos.y += m_moveSpeed * deltaTime;
        if (m_bodyPos.y >= m_maxY)
        {
            m_bodyPos.y = m_maxY;
            m_movingUp = false;
        }
    }
    else
    {
        m_bodyPos.y -= m_moveSpeed * deltaTime;
        if (m_bodyPos.y <= m_minY)
        {
            m_bodyPos.y = m_minY;
            m_movingUp = true;
        }
    }

    // ボス本体は体の上に乗るように追従
    m_bossPos = m_bodyPos + Vector3(0.0f, 120.0f, 0.0f);

    // モデル位置反映
    m_body.SetPosition(m_bodyPos);
    m_bossEnemy.SetPosition(m_bossPos);
}

/** 回転（見た目演出用、必要なら無効化可） */
void BossEnemy::Rotation()
{
    // 軽いY回転（演出）
    float rotDeg = 1.0f; // 1度/フレーム相当（調整可）
    Quaternion tmp;
    tmp.SetRotationDegY(rotDeg);
    m_rot.Multiply(m_rot, tmp); // 累積回転
    m_bossEnemy.SetRotation(m_rot);
}

/** コインを生成するユーティリティ（EnemyManagerが呼ぶ） */
void BossEnemy::SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ)
{
    // 修正：offsetX で rangeX を使う（以前のバグ修正）
    for (int i = 0; i < count; ++i)
    {
        float offsetX = ((float)std::rand() / RAND_MAX) * 2.0f * rangeX - rangeX;
        float offsetZ = ((float)std::rand() / RAND_MAX) * 2.0f * rangeZ - rangeZ;

        Vector3 pos = center;
        pos.x += offsetX;
        pos.z += offsetZ;
        pos.y += 10.0f;

        GetItem* coin = NewGO<GetItem>(0, "coin");
        coin->Init(pos);
    }
}

/** 敵が倒されたときに呼ぶ（EnemyManagerが呼ぶ） */
void BossEnemy::OnDefeated()
{
    if (!m_isAlive) return;

    // 無効化フラグ
    m_isAlive = false;

    // 衝突無効化（即座に当たりを外す）
    if (collision)
    {
        // collision を保持している場合は削除／無効化は管理者側で行う予定だが、
        // とりあえず当たり判定を無効化しておく
        collision->SetIsEnableAutoDelete(true);
        // DeleteGO(collision); // 実際の Delete は EnemyManager の削除ルーチンで行う
        collision = nullptr;
    }

    // 体・本体は描画しないようにしておく（描画は Update()/Renderで m_isAlive をチェック）
    // 追加のエフェクトやサウンドトリガーがあればここで呼ぶ
}

/** 描画 */
void BossEnemy::Render(RenderContext& rc)
{
    if (!m_isAlive) return;

    m_body.Draw(rc);
    m_bossEnemy.Draw(rc);
}
