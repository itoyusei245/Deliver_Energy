/**
 * @file EnemyManager.cpp
 * @brief 敵管理マネージャの実装
 */
#include "stdafx.h"
#include "Enemy/EnemyManager.h"
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

/**
 * @brief 更新処理（ボス戦のゲームループ管理）
 * @details 以下のフローを制御します。
 * 1. ボスからの「眷属生成リクエスト」を監視
 * 2. リクエストがあれば、中ボス的な「Familiar」と、巡回敵「NoobEnemy」を生成
 * - 生成位置や敵の種類は、ボスの段階（何回眷属を作ったか）によって変化します。
 * 3. 「Familiar」が倒されたか監視
 * 4. Familiar死亡時、ボスにダメージを与え、ボスが生きていれば次の生成フェーズへ移行
 */
void EnemyManager::Update()
{
    /*Player* player = FindGO<Player>("player");
    if (!player) return;*/


    if (m_boss) {
        // ... (コイン生成等のコメントアウトされた処理) ...

        // ---------------------------------------------------------
        // 眷属生成リクエストの処理
        // ---------------------------------------------------------
        // ボスが眷属生成を要求しており、かつ現在眷属がいない場合
        if (m_boss->IsRequestCreateFamiliar()) {

            // 安全対策：既にFamiliarがいるのに生成しようとしていないかチェック
            K2_ASSERT(m_familia == nullptr, "ファミリアがすでに作られています。\n");

            if (m_familia == nullptr) {
                // --- Familiar（ボスを守る中ボス）の生成 ---
                m_familia = NewGO<Familiar>(0, "familiar");

                // タイプを交互に切り替える（TypeA <-> TypeB）
                if (m_isChangeType) {
                    m_familia->Setup(enFamiliarType_A);
                    m_isChangeType = false;
                }
                else {
                    m_familia->Setup(enFamiliarType_B);
                    m_isChangeType = true;
                }

                // ボスがこれまでに何回眷属を作ったかを取得（戦闘フェーズの進行度）
                const int createdFamiliarNum = m_boss->GetStatus()->GetCreatedFamiliarNum();

                // 生成する雑魚敵（NoobEnemy）のタイプ範囲を計算（1フェーズにつき4種類ずつ）
                int startType = createdFamiliarNum * 4;
                int endType = startType + 4;

                // フェーズ（生成回数）に応じてFamiliarの出現位置を変更
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

                // --- NoobEnemy（巡回する雑魚敵）の生成 ---
                // 定義されている最大タイプ数を超えないようにガード
                if (endType > EnemyController::enEnemyType_Max) {
                    endType = EnemyController::enEnemyType_Max;
                }

                // 計算した範囲のタイプの敵をまとめて生成し、AIコントローラーを割り当てる
                for (int i = startType; i < endType; i++) {
                    NoobEnemy* noob = NewGO<NoobEnemy>(0, "nnobEnemy");
                    auto* ai = NewGO<EnemyController>(0, "enemyController");
                    ai->SetTarget(noob); // AIに操作対象をセット

                    // 敵タイプ（巡回ルート）を設定
                    ai->SetEnemyType(static_cast<EnemyController::EnEnemyType>(i));
                }
            }
            // ボス側のフラグを更新（生成完了通知）
            m_boss->SetCreatedFamiliar(true);
            m_boss->SetRequestCreateFamiliar(false);
        }
    }

    // ---------------------------------------------------------
    // ファミリアの死亡チェックとボスのダメージ処理
    // ---------------------------------------------------------
    if (m_familia) {
        // ファミリアが倒されたかチェック
        if (m_familia->GetStatus()->IsDead()) {
            // 倒されたら削除
            DeleteGO(m_familia);
            m_familia = nullptr;

            if (m_boss) {
                // ボス本体にダメージを与える
                // NOTE: プレイヤーは直接ボスを攻撃できず、眷属を倒すことでボスが傷つく仕組み
                m_boss->GetStatus()->Damage(1);

                if (m_boss->GetStatus()->IsDead()) {
                    // HPが0になったらボスを倒す（削除）
                    DeleteGO(m_boss);
                    m_boss = nullptr;
                }
                else {
                    // まだボスが倒されてないなら、フラグを折って「次の眷属を作ってよい」状態にする
                    m_boss->SetCreatedFamiliar(false);
                }
            }
        }
    }
}

void EnemyManager::Setup()
{
}

/**
 * @brief 敵（ボス）生成処理
 * @details ボス本体と、ボス用UIを生成します。
 */
void EnemyManager::CreateEnemy(const Vector3& pos)
{
    // UIの生成
    m_bossUI = NewGO<BossStatusUI>(0, "bossUI");

    // ボスの生成
    m_boss = NewGO<Boss>(0, "boss");
    m_boss->SetPosition(pos);
}