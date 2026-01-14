#include "stdafx.h"
#include "EnemyStatePattern.h"
#include "Boss.h"
#include "Familiar.h"
#include "Enemy/NoobEnemy.h"
#include "Enemy/EnemyManager.h"


namespace
{
    static const float BOSS_MIN_Y = -120.0f;
    static const float BOSS_MAX_Y = 80.0f;


    static const float FAMILIAR_OFFSET_Y = 140.0f;


    Vector3 ComputeNextPosition(const Vector3& position, const float maxY, const float minY, const float moveSpeed, bool& isMovingUp)
    {
        Vector3 nextPosition = position;

        nextPosition.y += isMovingUp ? moveSpeed : moveSpeed * -1.0f;
        if (isMovingUp) {
            nextPosition.y = min(nextPosition.y, maxY);
        }
        else {
            nextPosition.y = max(nextPosition.y, minY);
        }
        if (nextPosition.y >= maxY || nextPosition.y <= minY) {
            isMovingUp = !isMovingUp;
        }

        return nextPosition;
    }
}



void BossIdleState::Eneter()
{
	// アニメーション再生
}


void BossIdleState::Update()
{
}


void BossIdleState::Exit()
{
}


bool BossIdleState::RequestState(int& request)
{
    if (true) {
        request = enBossStateType_Move;
        return true;
    }
	return false;
}




void BossMoveState::Eneter()
{
    // アニメーション再生
}


void BossMoveState::Update()
{
    const float deltaTime = g_gameTime->GetFrameDeltaTime();

    const float moveSpeed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime;

    //Bossクラスから現在のフラグを取得
    bool isMovingUp = m_owner->IsMovingUp();


    Vector3 nextPosition = ComputeNextPosition(
        m_owner->GetPosition(),
        BOSS_MAX_Y, 
        BOSS_MIN_Y, 
        moveSpeed, 
        m_isMovingUp
    );


    m_owner->SetMovingUp(m_isMovingUp);
    m_owner->SetPosition(nextPosition);
}


void BossMoveState::Exit()
{

}


bool BossMoveState::RequestState(int& request)
{
    if (!m_owner->IsCreatedFamiliar()) {
        request = enBossStateType_CreateFamiliar;
        return true;
    }
    return false;
}




void BossCreateFamiliarState::Eneter()
{
    // 生成リクエスト
    m_owner->SetRequestCreateFamiliar(true);
}


void BossCreateFamiliarState::Update()
{
}


void BossCreateFamiliarState::Exit()
{

}


bool BossCreateFamiliarState::RequestState(int& request)
{
    if (m_owner->IsCreatedFamiliar()) {
        request = enBossStateType_Idle;
        return true;
    }
    return false;
}




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


void FamiliarMoveState::Update()
{
    const float deltaTime = g_gameTime->GetFrameDeltaTime();


    //Bossを取得
    Boss* boss = EnemyManager::GetInstance()->GetBoss();


    //Bossがいる場合、方向を同期させて速度を修正する
    if (boss) {
        m_isMovingUp = boss->IsMovingUp();


        float syncSpeedRatio = 140.0f / 200.0f;
        float moveSpeed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime * syncSpeedRatio;
        //float moveSpeed = m_owner->GetStatus()->GetMoveSpeed() * deltaTime;

        //Vector3 nextPosition = m_owner->GetPosition();

        //nextPosition.y += m_isMovingUp ? moveSpeed : moveSpeed * -1.0f;
        //if (m_isMovingUp) {
        //    nextPosition.y = min(nextPosition.y, _MAX_Y);
        //}
        //else {
        //    nextPosition.y = max(nextPosition.y, BOSS_MIN_Y);
        //}
        //if (nextPosition.y >= BOSS_MAX_Y || nextPosition.y <= BOSS_MIN_Y) {
        //    m_isMovingUp = !m_isMovingUp;
        //}

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
    return false;
}




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
    const Vector3 moveDirection = m_owner->m_moveVector;
    const Vector3 moveSpeed = moveDirection * speed;
    const Vector3 nextPosition = m_owner->m_position + moveSpeed;
    m_owner->SetPosition(nextPosition);

    //Vector3 nextPosition = m_owner->GetPosition();

    //nextPosition.y += m_isMovingUp ? moveSpeed : moveSpeed * -1.0f;
    //if (m_isMovingUp) {
    //    nextPosition.y = min(nextPosition.y, _MAX_Y);
    //}
    //else {
    //    nextPosition.y = max(nextPosition.y, BOSS_MIN_Y);
    //}
    //if (nextPosition.y >= BOSS_MAX_Y || nextPosition.y <= BOSS_MIN_Y) {
    //    m_isMovingUp = !m_isMovingUp;
    //}

    //Vector3 nextPosition = ComputeNextPosition(m_owner->GetPosition(), FAMILIAR_MAX_Y, FAMILIAR_MIN_Y, moveSpeed, m_isMovingUp);

    //m_owner->SetPosition(nextPosition);
}


void NoobEnemyMoveState::Exit()
{

}


bool NoobEnemyMoveState::RequestState(int& request)
{
    return false;
}