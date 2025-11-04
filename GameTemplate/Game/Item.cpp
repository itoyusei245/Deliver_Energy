#include "stdafx.h"
#include "Item.h"
#include "Player.h" 
#include "BackGround.h"

Item::Item()
{
}

Item::~Item()
{
    if (m_collision)
    {
        DeleteGO(m_collision);
        m_collision = nullptr;
    }
}

void Item::Init(const Vector3& pos)
{
    m_position = pos;
    m_velocity = { 0.0f, 0.0f, 0.0f };

    m_coin.Init("Assets/modelData/Item/coin.tkm");
    m_coin.SetPosition(m_position);

    m_collision = NewGO<CollisionObject>(0, "coinCollision");
    m_collision->CreateSphere(m_position, Quaternion::Identity, m_radius);
    m_collision->SetIsEnableAutoDelete(false);

    // 追加初期化
    m_spawnTime = 0.0f;
    m_blinkTimer = 0.0f;
    m_blinkStarted = false;
    m_isVisible = true;
}

void Item::Update()
{
    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_spawnTime += deltaTime;

    /**重力処理*/
    if (!m_isOnGround)
    {
        m_velocity.y += m_gravity;   /**落下加速度*/
        m_position += m_velocity * g_gameTime->GetFrameDeltaTime(); /**時間補正あり*/
    }

    /**地面との当たり判定*/
    Vector3 rayStart = m_position + Vector3(0.0f, 50.0f, 0.0f);
    Vector3 rayEnd = m_position + Vector3(0.0f, -1000.0f, 0.0f);  /**下方向に長いレイ*/
    Vector3 hitPos;

    /**PhysicsWorldのインスタンスを取得*/
    auto* physics = PhysicsWorld::GetInstance();

    if (physics->RayTest(rayStart, rayEnd, hitPos))
    {
        float groundY = hitPos.y;
        float coinBottomY = m_position.y - m_radius;

        /**地面より下に行ったら位置補正*/
        if (coinBottomY <= groundY)
        {
            m_isOnGround = true;
            m_velocity.y = 0.0f;
            m_position.y = groundY + m_radius;

            if (m_collision)
                m_collision->SetPosition(m_position);
        }
    }

    /**プレイヤーとの接触*/
    Player* player = FindGO<Player>("player");
    if (player && m_collision && m_collision->IsHit(player->characterController))
    {
        DeleteGO(this);
        return;
    }

    /**回転処理*/
    m_rotation += 1.0f;
    Quaternion rot;
    rot.SetRotationDegY(m_rotation);
    m_coin.SetRotation(rot);

    /**▼▼▼ 点滅処理（トグル方式）▼▼▼**/
    if (!m_blinkStarted && m_spawnTime >= 20.0f)
    {
        // 点滅開始時に初期化
        m_blinkStarted = true;
        m_blinkTimer = 0.0f;
    }

    if (m_blinkStarted && m_spawnTime < 25.0f)
    {
        float blinkPhase = m_spawnTime - 20.0f;

        // 点滅間隔をだんだん短く（1.0s → 0.05s）
        const float startInterval = 1.0f;
        const float endInterval = 0.05f;
        float t = blinkPhase / 5.0f;
        if (t > 1.0f) t = 1.0f;
        float blinkInterval = startInterval + (endInterval - startInterval) * t;

        // タイマー進行
        m_blinkTimer += deltaTime;

        // 一定間隔ごとにON/OFF切り替え
        if (m_blinkTimer >= blinkInterval)
        {
            m_isVisible = !m_isVisible;
            m_blinkTimer -= blinkInterval; // 残り時間を繰り越す（安定化）
        }
    }
    else if (m_spawnTime >= 25.0f)
    {
        DeleteGO(this);
        return;
    }
    /**▲▲▲ 点滅処理ここまで ▲▲▲**/

    if (m_isVisible)
    {
        m_coin.SetPosition(m_position);
        m_coin.Update();
    }
}

void Item::Render(RenderContext& rc)
{
    if (m_isVisible)
    {
        m_coin.Draw(rc);
    }
}
