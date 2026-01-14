#include "stdafx.h"
#include "Enemy/EnemyManager.h"
//#include "BossEnemy.h"
//#include "Player.h"
//#include "GetItem.h"
#include "Enemy/Boss.h"
#include "Enemy/Familiar.h"
#include "Enemy/BossStatusUI.h"
#include "Enemy/NoobEnemy.h"
#include "Enemy/EnemyController.h"

EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
    /*Player* player = FindGO<Player>("player");
    if (!player) return;*/

    
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
        // ---------------------------------------------------------
        // 眷属生成リクエストの処理
        // ---------------------------------------------------------
        if (m_boss->IsRequestCreateFamiliar()) {
            K2_ASSERT(m_familia == nullptr, "ファミリアがすでに作られています。\n");
            if (m_familia == nullptr) {
                // 眷属を交互に生成
                m_familia = NewGO<Familiar>(0, "familiar");
                if (m_isChangeType) {
                    m_familia->Setup(enFamiliarType_A);
                    m_isChangeType = false;
                }
                else {
                    m_familia->Setup(enFamiliarType_B);
                    m_isChangeType = true;
                }


                // 何体目かを取得して生成位置を変える
                const int createdFamiliarNum = m_boss->GetStatus()->GetCreatedFamiliarNum();

                //生成する雑魚敵の範囲を計算（1回につき4体ずつ）
                int startType = createdFamiliarNum * 4;
                int endType = startType + 4;


                if (createdFamiliarNum == 0)
                {
                    m_familia->SetPosition(Vector3(3400.0f, -100.0f, -3900.0f));
                    m_familia->SetDefaultPosition(Vector3(3400.0f, -100.0f, -3900.0f));
                }
                else if (createdFamiliarNum == 1)
                {
                    m_familia->SetPosition(Vector3(3400.0, -450.0f, -3500.0f));
                    m_familia->SetDefaultPosition(Vector3(3400.0, -450.0f, -3500.0f));
                }
                else if (createdFamiliarNum == 2)
                {
                    m_familia->SetPosition(Vector3(3400.0f, 700.0f, -4000.0f));
                    m_familia->SetDefaultPosition(Vector3(3400.0f, 700.0f, -4000.0f));
                }
                // 生成カウントを進める
                m_boss->GetStatus()->AddCreatedFamiliarNum();


                // 定義されている最大数を超えないようにガード
                if (endType > EnemyController::enEnemyType_Max) {
                    endType = EnemyController::enEnemyType_Max;
                }

                for (int i = startType; i < endType; i++) {
                    NoobEnemy* noob = NewGO<NoobEnemy>(0, "nnobEnemy");
                    auto* ai = NewGO<EnemyController>(0, "enemyController");
                    ai->SetTarget(noob);

                    ai->SetEnemyType(static_cast<EnemyController::EnEnemyType>(i));
                }
            }
            m_boss->SetCreatedFamiliar(true);
            m_boss->SetRequestCreateFamiliar(false);
        }
    }

    // ---------------------------------------------------------
    // ファミリアの死亡チェックとボスのダメージ処理
    // ---------------------------------------------------------
    if (m_familia) {
        // ファミリアが死んだかチェック
        if (m_familia->GetStatus()->IsDead()) {
            DeleteGO(m_familia);
            m_familia=nullptr;


            if (m_boss) {
                m_boss->GetStatus()->Damage(1);


                if (m_boss->GetStatus()->IsDead()) {
                    //HPが0になったらボスを倒す
                    DeleteGO(m_boss);
                    m_boss = nullptr;
                }
                else {
                    //まだボスが倒されてない状態なら「眷属を作って」と伝える
                    m_boss->SetCreatedFamiliar(false);
                }
            }
        }
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

    // UIの生成
    m_bossUI = NewGO<BossStatusUI>(0, "bossUI");
    m_boss = NewGO<Boss>(0, "boss");
    m_boss->SetPosition(pos);
}
