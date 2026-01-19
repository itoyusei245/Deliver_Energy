/**
 * @file EnemyStatePattern.cpp
 * @brief 敵ステートの実装
 */
#include "stdafx.h"
#include "EnemyStatePattern.h"
#include "Boss.h"
#include "Familiar.h"
#include "Enemy/NoobEnemy.h"
#include "Enemy/EnemyManager.h"


namespace
{
    // ボスの移動範囲定数
    static const float BOSS_MIN_Y = -120.0f;
    static const float BOSS_MAX_Y = 80.0f;

    // FamiliarのボスからのY座標オフセット
    static const float FAMILIAR_OFFSET_Y = 140.0f;

    /**
     * @brief 上下反復移動（ピンポン移動）の次位置を計算する関数
     * @param[in] position 現在の位置
     * @param[in] maxY 上限Y座標
     * @param[in] minY 下限Y座標
     * @param[in] moveSpeed 移動速度
     * @param[in,out] isMovingUp 上昇中かどうかのフラグ（参照渡しで反転時に書き換わる）
     * @return Vector3 新しい座標
     */
    Vector3 ComputeNextPosition(const Vector3& position, const float maxY, const float minY, const float moveSpeed, bool& isMovingUp)
    {
        Vector3 nextPosition = position;

        // フラグに応じて加算・減算
        nextPosition.y += isMovingUp ? moveSpeed : moveSpeed * -1.0f;

        // 範囲制限と反転処理
        if (isMovingUp) {
            nextPosition.y = min(nextPosition.y, maxY);
        }
        else {
            nextPosition.y = max(nextPosition.y, minY);
        }
        // 端に到達したら進行方向を反転
        if (nextPosition.y >= maxY || nextPosition.y <= minY) {
            isMovingUp = !isMovingUp;
        }

        return nextPosition;
    }
}


// -------------------------------------------------------------
// Boss Idle State
// -------------------------------------------------------------
void BossIdleState::Eneter()
{
    // アニメーション再生（必要であれば実装）
}


void BossIdleState::Update()
{
}


void BossIdleState::Exit()
{
}


bool BossIdleState::RequestState(int& request)
{
    // 即座に移動ステートへ遷移する
    if (true) {
        request = enBossStateType_Move;
        return true;
    }
    return false;
}


// -------------------------------------------------------------
// Boss Move State
// -------------------------------------------------------------
void BossMoveState::Eneter()
{
    // アニメーション再生
}


void BossMoveState::Update()
{
    const float deltaTime = g_gameTime->GetFrameDeltaTime();
    const float moveSpeed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime;

    // Bossクラスから現在の移動方向フラグを取得（状態を保持・復帰するため）
    bool isMovingUp = m_owner->IsMovingUp();

    // 次の座標を計算
    Vector3 nextPosition = ComputeNextPosition(
        m_owner->GetPosition(),
        BOSS_MAX_Y,
        BOSS_MIN_Y,
        moveSpeed,
        m_isMovingUp // このメンバ変数はUpdate内でローカルに使われているようだが、本来はm_owner->IsMovingUp()と同期すべき箇所の可能性あり
    );

    // 計算後のフラグと座標をBoss本体に反映
    m_owner->SetMovingUp(m_isMovingUp);
    m_owner->SetPosition(nextPosition);
}


void BossMoveState::Exit()
{
}


bool BossMoveState::RequestState(int& request)
{
    // 眷属が生成されていない場合、生成ステートへ遷移する
    // これにより「移動 -> 眷属がいなければ召喚 -> 移動」というサイクルを作る
    if (!m_owner->IsCreatedFamiliar()) {
        request = enBossStateType_CreateFamiliar;
        return true;
    }
    return false;
}


// -------------------------------------------------------------
// Boss Create Familiar State
// -------------------------------------------------------------
void BossCreateFamiliarState::Eneter()
{
    // ボスに対して「眷属を作ってくれ」というフラグを立てる
    // 実際の生成処理は EnemyManager がこのフラグを見て行う
    m_owner->SetRequestCreateFamiliar(true);
}


void BossCreateFamiliarState::Update()
{
    // EnemyManagerが生成を完了するのを待つ
}


void BossCreateFamiliarState::Exit()
{
}


bool BossCreateFamiliarState::RequestState(int& request)
{
    // 眷属の生成が完了したフラグが立ったら、Idleステートに戻る
    if (m_owner->IsCreatedFamiliar()) {
        request = enBossStateType_Idle;
        return true;
    }
    return false;
}


/*************************************************************/
// Familiar States
/*************************************************************/

void FamiliarIdleState::Eneter()
{
    // アニメーション再生
}


void FamiliarIdleState::Update()
{
}


void FamiliarIdleState::Exit()
{
}


bool FamiliarIdleState::RequestState(int& request)
{
    // 即座に移動ステートへ遷移
    if (true) {
        request = enFamiliarStateType_Move;
        return true;
    }
    return false;
}


void FamiliarMoveState::Eneter()
{
    // アニメーション再生
}


/**
 * @brief Familiarの移動更新
 * @details ボスが存在する場合、ボスの移動方向と同期して動きます。
 */
void FamiliarMoveState::Update()
{
    const float deltaTime = g_gameTime->GetFrameDeltaTime();

    // EnemyManager経由でBossを取得
    Boss* boss = EnemyManager::GetInstance()->GetBoss();

    // Bossがいる場合、方向を同期させて速度を修正する
    if (boss) {
        // ボスの移動方向をコピー（これでボスと同じタイミングで上下する）
        m_isMovingUp = boss->IsMovingUp();

        // ボスとの移動距離比率などで速度を調整
        float syncSpeedRatio = 140.0f / 200.0f;
        float moveSpeed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime * syncSpeedRatio;

        // 次の座標を計算
        // ここでは m_isMovingUp は計算に使われるが、反転ロジック自体はボスのフラグに依存しているため
        // ComputeNextPosition内での反転はあまり意味を持たないかもしれない（ボスが反転すればこっちも反転する）
        Vector3 nextPosition = ComputeNextPosition(
            m_owner->GetPosition(),
            m_owner->GetDefaultPosition().y + FAMILIAR_OFFSET_Y,
            m_owner->GetDefaultPosition().y,
            moveSpeed,
            m_isMovingUp
        );

        m_owner->SetPosition(nextPosition);
    }
}


void FamiliarMoveState::Exit()
{
}


bool FamiliarMoveState::RequestState(int& request)
{
    // 遷移しない（ずっと動き続ける）
    return false;
}


/*************************************************************/
// NoobEnemy States
/*************************************************************/

void NoobEnemyIdleState::Eneter()
{
    // アニメーション再生
}


void NoobEnemyIdleState::Update()
{
}


void NoobEnemyIdleState::Exit()
{
}


bool NoobEnemyIdleState::RequestState(int& request)
{
    // 即座に移動へ
    if (true) {
        request = enNoobEnemyStateType_Move;
        return true;
    }
    return false;
}


void NoobEnemyMoveState::Eneter()
{
    // アニメーション再生
}


void NoobEnemyMoveState::Update()
{
    const float deltaTime = g_gameTime->GetFrameDeltaTime();
    float speed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime;

    // AI(EnemyController)によってセットされた移動ベクトルに基づいて移動
    const Vector3 moveDirection = m_owner->m_moveVector;
    const Vector3 moveSpeed = moveDirection * speed;
    const Vector3 nextPosition = m_owner->m_position + moveSpeed;

    m_owner->SetPosition(nextPosition);
}


void NoobEnemyMoveState::Exit()
{
}


bool NoobEnemyMoveState::RequestState(int& request)
{
    return false;
}