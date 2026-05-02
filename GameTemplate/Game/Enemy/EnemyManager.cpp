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
#include "Sound/SoundManager.h"
#include "graphics/effect/EffectEmitter.h"
#include "GetItem.h"

namespace 
{
	constexpr const char* NAME_EFF_EXPRO_IN = "eff_expro_in";
	constexpr const char* NAME_EFF_KYUSYU   = "eff_kyusyu";
	constexpr const char* NAME_EFF_EXPRO_M  = "eff_expro_m";
	constexpr const char* NAME_EFF_FAMILIAR = "familiarDeathEffect";
	constexpr const char* NAME_COIN         = "coin";
	constexpr const char* NAME_FAMILIAR     = "familiar";
	constexpr const char* NAME_NOOB_ENEMY    = "noobEnemy";
	constexpr const char* NAME_ENEMY_CTRL   = "enemyController";
	constexpr const char* NAME_BOSS_UI      = "bossUI";
	constexpr const char* NAME_BOSS         = "boss";

    constexpr int FRAME_BOSS_DEATH_EXPRO_IN   = 0;   
    constexpr int FRAME_BOSS_DEATH_KYUSYU     = 120;
    constexpr int FRAME_BOSS_DEATH_EXPRO_M    = 360;
    constexpr int FRAME_BOSS_DEATH_VANISH     = 420;
    constexpr int FRAME_BOSS_DEATH_COIN_START = 420;
    constexpr int FRAME_BOSS_DEATH_COIN_END   = 440;
    constexpr int FRAME_BOSS_DEATH_FINISH     = 480;

	const Vector3 SCALE_EFF_BOSS(100.0f, 100.0f, 100.0f);
	const Vector3 SCALE_EFF_FAMILIAR(50.0f, 50.0f, 50.0f);
	constexpr float BOSS_SINK_SPEED = 0.5f;

	const Vector3 POS_FAMILIAR_PHASE_0(3400.0f, -100.0f, -3900.0f);
	const Vector3 POS_FAMILIAR_PHASE_1(3400.0f, -450.0f, -3500.0f);
	const Vector3 POS_FAMILIAR_PHASE_2(3400.0f,  700.0f, -4000.0f);

	constexpr int WAIT_FRAME_FAMILIAR_DEATH = 30;
}

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
    // ---------------------------------------------------------
    // ボス本体の死亡と遅延処理
    // ---------------------------------------------------------
    if (m_isBossDying || (m_boss && m_boss->GetStatus()->IsDead()))
    {
        // ボスが沈んでもエフェクトやコインが空中の同じ場所から出るように、最初の位置を記憶する
        static Vector3 bossDeathPos = Vector3::Zero;

        // 死亡した瞬間に1回だけ行う処理
        if (!m_isBossDying) {
            m_isBossDying = true;
            m_bossDeathTimer = 0;

            SoundManager::Get().PlaySE(enSoundKind_DieBoss);
            SoundManager::Get().StopBGM();
            SoundManager::Get().PlayBGM(enSoundKind_Normal);

            // カメラのクリア演出開始（ガガガというシェイクもここで開始）
            m_isBossClearEventRequested = true;

            // ボスが沈み始める「前」の、最初の座標をここで記憶しておく
            bossDeathPos = m_boss->GetPosition();
        }

        // ---------------------------------------------------
        // ボスの座標を下に沈ませる（Transform移動）
        // ---------------------------------------------------
        if (m_boss != nullptr) {
            Vector3 pos = m_boss->GetPosition();
            pos.y -= BOSS_SINK_SPEED; 
            m_boss->SetPosition(pos);
        }


        // 0秒時点（0フレーム）：複数爆発 (expro_in.efk)
        if (m_bossDeathTimer == FRAME_BOSS_DEATH_EXPRO_IN) {
            EffectEmitter* effectIn = NewGO<EffectEmitter>(0, NAME_EFF_EXPRO_IN);
            effectIn->Init(1);
            Vector3 pos = bossDeathPos; 
            pos.y += 100.0f;
            effectIn->SetPosition(pos);
            effectIn->SetScale(SCALE_EFF_BOSS);
            effectIn->Play();
            effectIn->Update();
        }

        // 2秒時点（120フレーム）：溜めエフェクト (kyusyu.efk)
        if (m_bossDeathTimer == FRAME_BOSS_DEATH_KYUSYU) {
            EffectEmitter* effectKyusyu = NewGO<EffectEmitter>(0, NAME_EFF_KYUSYU);
            effectKyusyu->Init(2);
            Vector3 pos = bossDeathPos; 
            pos.y += 100.0f;
            effectKyusyu->SetPosition(pos);
            effectKyusyu->SetScale(SCALE_EFF_BOSS);
            effectKyusyu->Play();
            effectKyusyu->Update();
        }

        // 6秒時点（360フレーム）：デカ爆発 (expro_M.efk)
        if (m_bossDeathTimer == FRAME_BOSS_DEATH_EXPRO_M) {
            EffectEmitter* effectBig = NewGO<EffectEmitter>(0, NAME_EFF_EXPRO_M);
            effectBig->Init(3);
            Vector3 pos = bossDeathPos; 
            pos.y += 100.0f;
            effectBig->SetPosition(pos);
            effectBig->SetScale(SCALE_EFF_BOSS);
            effectBig->Play();
            effectBig->Update();
        }

        // ---------------------------------------------------
        // 7秒時点（420フレーム）で「ボス本体」だけを先に消滅させる
        // ---------------------------------------------------
        if (m_bossDeathTimer == FRAME_BOSS_DEATH_VANISH) {
            if (m_boss != nullptr) {
                DeleteGO(m_boss);
                m_boss = nullptr;
            }
        }

        // ---------------------------------------------------
        // コインの分散生成（ボスが消えた直後からスタート）
        // 420フレーム から 440フレーム までの間に100枚ドロップ
        // ---------------------------------------------------
        if (m_bossDeathTimer >= FRAME_BOSS_DEATH_VANISH && m_bossDeathTimer < FRAME_BOSS_DEATH_COIN_END) {
            for (int i = 0; i < 5; ++i) {
                GetItem* coin = NewGO<GetItem>(0, NAME_COIN);
                Vector3 coinPos = bossDeathPos; 

                // ボスの中心より少し高めから出現
                coinPos.y += 1000.0f;
                coinPos.x += (rand() % 1500) - 750.0f;
                coinPos.z += (rand() % 1500) - 750.0f;

                coin->Init(coinPos);
            }
        }

        // タイマーを進める
        m_bossDeathTimer++;

        // 演出の完全終了（8秒＝480フレーム）でフラグをリセット
        if (m_bossDeathTimer >= FRAME_BOSS_DEATH_FINISH) {
            m_isBossDying = false;
        }

        return; // 演出中は以下の更新を行わずに処理を抜ける
    }

    if (m_boss) {
        // ---------------------------------------------------------
        // 眷属生成リクエストの処理
        // ---------------------------------------------------------
        // ボスが眷属生成を要求しており、かつ現在眷属がいない場合
        if (m_boss->IsRequestCreateFamiliar()) {

            // 安全対策：既にFamiliarがいるのに生成しようとしていないかチェック
            K2_ASSERT(m_familia == nullptr, "ファミリアがすでに作られています。\n");

            if (m_familia == nullptr) {
                // --- Familiar（ボスを守る中ボス）の生成 ---
                m_familia = NewGO<Familiar>(0, NAME_FAMILIAR);

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
                    m_familia->SetPosition(POS_FAMILIAR_PHASE_0);
                    m_familia->SetDefaultPosition(POS_FAMILIAR_PHASE_0);
                }
                else if (createdFamiliarNum == 1)
                {
                    m_familia->SetPosition(POS_FAMILIAR_PHASE_1);
                    m_familia->SetDefaultPosition(POS_FAMILIAR_PHASE_1);
                }
                else if (createdFamiliarNum == 2)
                {
                    m_familia->SetPosition(POS_FAMILIAR_PHASE_2);
                    m_familia->SetDefaultPosition(POS_FAMILIAR_PHASE_2);
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
                    NoobEnemy* noob = NewGO<NoobEnemy>(0, NAME_NOOB_ENEMY);
                    auto* ai = NewGO<EnemyController>(0, NAME_ENEMY_CTRL);
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
        if (m_familia->GetStatus()->IsDead()) {

            // 1. 死亡した瞬間の処理（1回だけ）
            if (!m_isFamiliarDying) {
                m_isFamiliarDying = true;
                m_familiarDeathTimer = 0; // タイマー初期化
                m_familia->OnDead();

                // 爆発エフェクト発生
                EffectEmitter* effect = NewGO<EffectEmitter>(0, NAME_EFF_FAMILIAR);
                effect->Init(0);
                Vector3 effectPos = m_familia->GetPosition();
                effect->SetPosition(effectPos);
                effect->SetScale(SCALE_EFF_FAMILIAR);
                effect->Play();
                effect->Update();
            }

            // タイマーを進めて待つ
            m_familiarDeathTimer++;

            // tフレームを進めて削除
            if (m_familiarDeathTimer >= WAIT_FRAME_FAMILIAR_DEATH) {

                DeleteGO(m_familia);
                m_familia = nullptr;

                // 次の眷属が生まれた時のためにフラグを戻す
                m_isFamiliarDying = false;

                if (m_boss) {
                    // ボス本体にダメージを与える
                    m_boss->GetStatus()->Damage(1);

                    if (!m_boss->GetStatus()->IsDead()){
                        m_boss->SetCreatedFamiliar(false);
                    }
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
