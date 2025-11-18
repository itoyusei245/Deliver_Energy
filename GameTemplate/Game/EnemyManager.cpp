#include "stdafx.h"
#include "EnemyManager.h"
#include "BossEnemy.h"
#include "Player.h"
#include "GetItem.h"

EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
    Player* player = FindGO<Player>("player");
    if (!player) return;

    std::vector<BossEnemy*> deadList;

    for (BossEnemy* boss : m_bossList)
    {
        if (!boss) continue;

        CollisionObject* core = boss->GetCoreCollider();
        if (core && core->IsHit(player->characterController))
        {
            /**コイン生成（BossEnemy 内の SpawnCoins を使用*/
            boss->SpawnCoins(
                boss->GetCorePosition(),
                50,
                1000.0f,
                1000.0f
            );

            deadList.push_back(boss);
        }
    }

    /**BossEnemy の削除処理*/
    for (BossEnemy* boss : deadList)
    {
        DeleteGO(boss);

        auto it = std::find(m_bossList.begin(), m_bossList.end(), boss);
        if (it != m_bossList.end())
            m_bossList.erase(it);
    }
}

void EnemyManager::Setup()
{
}

void EnemyManager::CreateEnemy(const Vector3& pos)
{
    BossEnemy* boss = NewGO<BossEnemy>(0, "bossEnemy");
    boss->SetBossPosition(pos);

    m_bossList.push_back(boss);
}
