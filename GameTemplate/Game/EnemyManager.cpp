#include "stdafx.h"
#include "EnemyManager.h"
#include "BossEnemy.h"
#include "Player.h"
#include "GetItem.h"
#include "Enemy/Boss.h"
#include "Enemy/Familiar.h"

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

    
    if (m_boss) {
        //CollisionObject* core = m_boss->GetCoreCollider();
        //if (core && core->IsHit(player->characterController))
        //{
        //    /**コイン生成（BossEnemy 内の SpawnCoins を使用*/
        //    //m_boss->SpawnCoins(
        //    //    boss->GetCorePosition(),
        //    //    50,
        //    //    1000.0f,
        //    //    1000.0f
        //    //);
        //}
        // 眷属生成リクエストされたか
        if (m_boss->IsRequestCreateFamiliar()) {
            K2_ASSERT(m_familia == nullptr, "ファミリアがすでに作られています。\n");
            if (m_familia == nullptr) {
                m_familia = NewGO<Familiar>(0, "familiar");
                const int createdFamiliarNum = m_boss->GetStatus()->GetCreatedFamiliarNum();
                if (createdFamiliarNum == 0)
                {
                    m_familia->SetPosition(Vector3(3400.0f, 100.0f, -3900.0f));
                }
                else if (createdFamiliarNum == 1)
                {
                    m_familia->SetPosition(Vector3(0.0f, 10.0f, 1000.0f));
                }
                else
                {
                    m_familia->SetPosition(Vector3(0.0f, 10.0f, 1000.0f));
                }
                m_boss->GetStatus()->AddCreatedFamiliarNum();
            }
            m_boss->SetCreatedFamiliar(true);
            m_boss->SetRequestCreateFamiliar(false);
        }
    }

    if (m_familia) {
        // ファミリアが死んだかチェック
    }
}

void EnemyManager::Setup()
{
}

void EnemyManager::CreateEnemy(const Vector3& pos)
{
    /*BossEnemy* boss = NewGO<BossEnemy>(0, "bossEnemy");
    boss->SetBossPosition(pos);

    m_bossList.push_back(boss);*/


    m_boss = NewGO<Boss>(0, "boss");
    m_boss->SetPosition(pos);
}
