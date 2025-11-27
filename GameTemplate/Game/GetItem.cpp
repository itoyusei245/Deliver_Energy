#include "stdafx.h"
#include "GetItem.h"
#include "Player.h" 
#include "BackGround.h"
#include"Game.h"

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
    m_velocity = { 0.0f, 0.0f, 0.0f };


    m_coin.Init("Assets/modelData/Item/coin.tkm");
    m_coin.SetPosition(m_position);


    m_collision = NewGO<CollisionObject>(0, "coinCollision");
    m_collision->CreateSphere(m_position, Quaternion::Identity, m_radius);
    m_collision->SetIsEnableAutoDelete(false);

    
    m_spawnTime = 0.0f;
    m_blinkTimer = 0.0f;
    m_blinkStarted = false;
    m_isVisible = true;
}

void GetItem::Update()
{
    if (Game::IsPaused) return;


    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_spawnTime += deltaTime;

   
    if (!m_isOnGround)
    {
        m_velocity.y += m_gravity;   
        m_position += m_velocity * g_gameTime->GetFrameDeltaTime(); 
    }

 
    Vector3 rayStart = m_position + Vector3(0.0f, 50.0f, 0.0f);
    Vector3 rayEnd = m_position + Vector3(0.0f, -1000.0f, 0.0f);  
    Vector3 hitPos;

    
    auto* physics = PhysicsWorld::GetInstance();

    if (physics->RayTest(rayStart, rayEnd, hitPos))
    {
        float groundY = hitPos.y;
        float coinBottomY = m_position.y - m_radius;

       
        if (coinBottomY <= groundY)
        {
            m_isOnGround = true;
            m_velocity.y = 0.0f;
            m_position.y = groundY + m_radius;

            if (m_collision)
                m_collision->SetPosition(m_position);
        }
    }


    Player* player = FindGO<Player>("player");
    if (player && m_collision && m_collision->IsHit(player->characterController))
    {
        m_totalCoinCount++;


        DeleteGO(this);
        return;
    }

  
    m_rotation += 1.0f;
    Quaternion rot;
    rot.SetRotationDegY(m_rotation);
    m_coin.SetRotation(rot);

    
    if (!m_blinkStarted && m_spawnTime >= 20.0f)
    {
        m_blinkStarted = true;
        m_blinkTimer = 0.0f;
    }
    if (m_blinkStarted && m_spawnTime < 25.0f)
    {
        float blinkPhase = m_spawnTime - 20.0f;

       
        const float startInterval = 1.0f;
        const float endInterval = 0.05f;
        float t = blinkPhase / 5.0f;
        if (t > 1.0f) t = 1.0f;
        float blinkInterval = startInterval + (endInterval - startInterval) * t;

       
        m_blinkTimer += deltaTime;

       
        if (m_blinkTimer >= blinkInterval)
        {
            m_isVisible = !m_isVisible;
            m_blinkTimer -= blinkInterval; 
        }
    }
    else if (m_spawnTime >= 25.0f)
    {
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
