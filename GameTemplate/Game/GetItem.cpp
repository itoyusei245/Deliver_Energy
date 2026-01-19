/**
 * @file GetItem.cpp
 * @brief コインアイテムの実装
 */
#include "stdafx.h"
#include "GetItem.h"
#include "Player.h" 
#include "BackGround.h"
#include"Game.h"

 // 静的メンバ変数の実体定義
int GetItem::m_totalCoinCount = 0;

GetItem::GetItem()
{
}

GetItem::~GetItem()
{
    // コリジョンは手動生成しているため、ここで削除
    if (m_collision)
    {
        DeleteGO(m_collision);
        m_collision = nullptr;
    }
}


/**
 * @brief 初期化処理
 * @details
 * モデルのロード、初期位置設定、当たり判定（Sphere）の生成を行います。
 * また、自動消滅管理用のタイマーもリセットします。
 */
void GetItem::Init(const Vector3& pos)
{
    m_position = pos;
    m_velocity = { 0.0f, 0.0f, 0.0f };

    // コインモデルの初期化
    m_coin.Init("Assets/modelData/Item/coin.tkm");
    m_coin.SetPosition(m_position);

    // 当たり判定の生成（半径20.0fの球）
    m_collision = NewGO<CollisionObject>(0, "coinCollision");
    m_collision->CreateSphere(m_position, Quaternion::Identity, m_radius);

    // コイン自体が消えてもコリジョンが残らないよう、削除管理はGetItem側で行う
    m_collision->SetIsEnableAutoDelete(false);

    // タイマー等の初期化
    m_spawnTime = 0.0f;
    m_blinkTimer = 0.0f;
    m_blinkStarted = false;
    m_isVisible = true;
}

/**
 * @brief 更新処理
 * @details
 * 1. 物理挙動：重力で落下し、下方向へのRayTestで地面を検知して着地させます。
 * 2. 接触判定：プレイヤーと接触したら、総数(m_totalCoinCount)を増やして自身を削除します。
 * 3. アニメーション：Y軸回転させます。
 * 4. 寿命管理：
 * - 20秒経過：点滅開始
 * - 20～25秒：徐々に点滅間隔を早める（1.0秒 -> 0.05秒間隔）
 * - 25秒経過：時間切れで消滅（削除）
 */
void GetItem::Update()
{
    if (Game::IsPaused) return;


    float deltaTime = g_gameTime->GetFrameDeltaTime();
    m_spawnTime += deltaTime;

    // --- 1. 物理挙動（落下） ---
    if (!m_isOnGround)
    {
        m_velocity.y += m_gravity;
        m_position += m_velocity * g_gameTime->GetFrameDeltaTime();
    }

    // --- 接地判定（RayTest） ---
    // 少し上から真下に向かってレイを飛ばす
    Vector3 rayStart = m_position + Vector3(0.0f, 50.0f, 0.0f);
    Vector3 rayEnd = m_position + Vector3(0.0f, -1000.0f, 0.0f);
    Vector3 hitPos;

    auto* physics = PhysicsWorld::GetInstance();

    if (physics->RayTest(rayStart, rayEnd, hitPos))
    {
        float groundY = hitPos.y;
        float coinBottomY = m_position.y - m_radius;

        // コインの底が地面より下に行ったら着地とみなす
        if (coinBottomY <= groundY)
        {
            m_isOnGround = true;
            m_velocity.y = 0.0f;
            // 地面にめり込まない位置に補正
            m_position.y = groundY + m_radius;

            // コリジョン位置も同期
            if (m_collision)
                m_collision->SetPosition(m_position);
        }
    }


    // --- 2. プレイヤーとの接触判定 ---
    Player* player = FindGO<Player>("player");
    if (player && m_collision && m_collision->IsHit(player->characterController))
    {
        // コイン獲得
        m_totalCoinCount++;

        // 自身を削除
        DeleteGO(this);
        return;
    }

    // --- 3. 回転アニメーション ---
    m_rotation += 1.0f;
    Quaternion rot;
    rot.SetRotationDegY(m_rotation);
    m_coin.SetRotation(rot);


    // --- 4. 点滅・消滅ロジック ---

    // 20秒経過で点滅フラグON
    if (!m_blinkStarted && m_spawnTime >= 20.0f)
    {
        m_blinkStarted = true;
        m_blinkTimer = 0.0f;
    }

    // 点滅期間（20秒～25秒）
    if (m_blinkStarted && m_spawnTime < 25.0f)
    {
        float blinkPhase = m_spawnTime - 20.0f; // 点滅開始からの経過時間

        // 点滅間隔を徐々に短くする計算
        // t: 0.0(開始時) ～ 1.0(終了直前)
        const float startInterval = 1.0f;  // 点滅初期の間隔
        const float endInterval = 0.05f;   // 点滅終盤の間隔
        float t = blinkPhase / 5.0f;
        if (t > 1.0f) t = 1.0f;

        // 現在のフェーズに応じた点滅間隔を算出
        float blinkInterval = startInterval + (endInterval - startInterval) * t;

        m_blinkTimer += deltaTime;

        // 指定間隔が経過したら表示状態を反転
        if (m_blinkTimer >= blinkInterval)
        {
            m_isVisible = !m_isVisible;
            m_blinkTimer -= blinkInterval;
        }
    }
    // 25秒経過で強制消滅
    else if (m_spawnTime >= 25.0f)
    {
        DeleteGO(this);
        return;
    }

    // 表示フラグが立っている時だけモデル位置を更新して描画準備
    if (m_isVisible)
    {
        m_coin.SetPosition(m_position);
        m_coin.Update();
    }
}


void GetItem::Render(RenderContext& rc)
{
    // 点滅OFFのタイミングでは描画しない
    if (m_isVisible)
    {
        m_coin.Draw(rc);
    }
}