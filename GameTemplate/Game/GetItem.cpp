/**
 * @file GetItem.cpp
 * @brief コインアイテムの実装
 */
#include "stdafx.h"
#include "GetItem.h"
#include "Player.h" 
#include "BackGround.h"
#include "Game.h"

namespace {
    constexpr const char* PATH_COIN_MODEL = "Assets/modelData/Item/coin.tkm";
    constexpr const char* NAME_COLLISION = "coinCollision";
    constexpr const char* NAME_PLAYER = "player";

    constexpr float COLLISION_RADIUS = 20.0f;
    constexpr float RAY_START_Y_OFFSET = 50.0f;
    constexpr float RAY_END_Y_OFFSET = -1000.0f;
    constexpr float RAY_X_OFFSET = 25.0f;
    constexpr float MODEL_HEIGHT_OFFSET = 40.0f;
    constexpr float FALL_THRESHOLD = 10.0f; 

    constexpr float ROTATION_SPEED = 1.0f; 

    constexpr float TIME_START_BLINK = 20.0f; 
    constexpr float TIME_DESPAWN = 25.0f; 
    constexpr float BLINK_DURATON = TIME_DESPAWN - TIME_START_BLINK; 

    constexpr float BLINK_INTERVAL_MAX = 1.0f;
    constexpr float BLINK_INTERVAL_MIN = 0.05f; 

    constexpr float SE_COOLDOWN_TIME = 0.05f; 
}

// 静的メンバ変数の実体定義
int GetItem::m_totalCoinCount = 0;

GetItem::GetItem()
{
}

GetItem::~GetItem()
{
    if (m_collision)
    {
        DeleteGO(m_collision);
        m_collision = nullptr;
    }
}

void GetItem::Init(const Vector3& pos)
{
    m_position = pos;
    m_velocity = Vector3::Zero;

    m_coin.Init(PATH_COIN_MODEL);
    m_coin.SetPosition(m_position);

    m_collision = NewGO<CollisionObject>(0, NAME_COLLISION);
    m_collision->CreateSphere(m_position, Quaternion::Identity, COLLISION_RADIUS);
    m_collision->SetIsEnableAutoDelete(false);

    m_spawnTime = 0.0f;
    m_blinkTimer = 0.0f;
    m_blinkStarted = false;
    m_isVisible = true;
    m_isDead = false;
}

void GetItem::Update()
{
    if (Game::IsPaused) return;
    if (m_isDead) return;

    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_spawnTime += deltaTime;

    // --- SEのクールダウン管理 ---
    static float seCooldown = 0.0f;
    if (seCooldown > 0.0f) {
        seCooldown -= deltaTime;
    }

    auto* physics = PhysicsWorld::GetInstance();
    Vector3 rayStart = m_position + Vector3(RAY_X_OFFSET, RAY_START_Y_OFFSET, 0.0f);
    Vector3 rayEnd = m_position + Vector3(RAY_X_OFFSET, RAY_END_Y_OFFSET, 0.0f);
    Vector3 hitPos;

    // --- 1. 物理挙動（落下と接地） ---
    if (!m_isOnGround)
    {
        m_velocity.y -= m_gravity;
        m_position += m_velocity * deltaTime;

        if (physics->RayTest(rayStart, rayEnd, hitPos))
        {
            float groundY = hitPos.y;
            float coinBottomY = m_position.y - MODEL_HEIGHT_OFFSET;

            if (coinBottomY <= groundY)
            {
                m_isOnGround = true;
                m_velocity.y = 0.0f;
                m_position.y = groundY + MODEL_HEIGHT_OFFSET;

                if (m_collision) {
                    m_collision->SetPosition(m_position);
                }
            }
        }
    }
    else
    {
        // 着地後、足場が消えていないか監視
        if (physics->RayTest(rayStart, rayEnd, hitPos))
        {
            float groundY = hitPos.y;
            float coinBottomY = m_position.y - MODEL_HEIGHT_OFFSET;

            if (groundY < coinBottomY - FALL_THRESHOLD)
            {
                m_isOnGround = false;
            }
        }
        else
        {
            m_isOnGround = false;
        }
    }

    // --- 2. プレイヤーとの接触判定 ---
    Player* player = FindGO<Player>(NAME_PLAYER);
    if (player && m_collision && m_collision->IsHit(player->characterController))
    {
        m_isDead = true;
        m_totalCoinCount++;

        if (seCooldown <= 0.0f) {
            SoundManager::Get().PlaySE(enSoundKind_Coin);
            seCooldown = SE_COOLDOWN_TIME;
        }

        if (m_collision) {
            DeleteGO(m_collision);
            m_collision = nullptr;
        }
        DeleteGO(this);
        return;
    }

    // --- 3. 回転アニメーション ---
    m_rotation += ROTATION_SPEED;
    Quaternion rot;
    rot.SetRotationDegY(m_rotation);
    m_coin.SetRotation(rot);

    // --- 4. 点滅・消滅ロジック ---
    if (!m_blinkStarted && m_spawnTime >= TIME_START_BLINK)
    {
        m_blinkStarted = true;
        m_blinkTimer = 0.0f;
    }

    if (m_blinkStarted && m_spawnTime < TIME_DESPAWN)
    {
        float blinkPhase = m_spawnTime - TIME_START_BLINK;

        float t = blinkPhase / BLINK_DURATON;
        if (t > 1.0f) t = 1.0f;

        float blinkInterval = BLINK_INTERVAL_MAX + (BLINK_INTERVAL_MIN - BLINK_INTERVAL_MAX) * t;

        m_blinkTimer += deltaTime;
        if (m_blinkTimer >= blinkInterval)
        {
            m_isVisible = !m_isVisible;
            m_blinkTimer -= blinkInterval;
        }
    }
    else if (m_spawnTime >= TIME_DESPAWN)
    {
        m_isDead = true;
        if (m_collision) {
            DeleteGO(m_collision);
            m_collision = nullptr;
        }
        DeleteGO(this);
        return;
    }

    if (m_isVisible)
    {
        m_coin.SetPosition(m_position);
        m_coin.Update();
    }
}

void GetItem::Render(RenderContext& rc)
{
    if (m_isVisible)
    {
        m_coin.Draw(rc);
    }
}