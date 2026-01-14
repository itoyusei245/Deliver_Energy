#include "stdafx.h"
#include "AthleticState.h"
#include "AthleticStage.h"

namespace
{
	// 上下移動の次の位置を計算する関数
	Vector3 ComputeNextPosition(const Vector3& position, const float maxY, const float minY, const float moveSpeed, bool& isMovingUp)
	{
		Vector3 nextPosition = position;

		// 上昇中ならプラス、下降中ならマイナス
		nextPosition.y += isMovingUp ? moveSpeed : moveSpeed * -1.0f;

		// 上限・下限チェックと反転
		if (isMovingUp) {
			if (nextPosition.y >= maxY) {
				nextPosition.y = maxY;
				isMovingUp = false; // 下降へ切り替え
			}
		}
		else {
			if (nextPosition.y <= minY) {
				nextPosition.y = minY;
				isMovingUp = true; // 上昇へ切り替え
			}
		}

		return nextPosition;
	}
}

// --------------------------------------------------------
// Idle State
// --------------------------------------------------------
void AthleticIdleState::Enter()
{
}

void AthleticIdleState::Update()
{
	// 待機中は何もしない
}

void AthleticIdleState::Exit()
{
}

bool AthleticIdleState::RequestState(int& request)
{
	// すぐに移動ステートへ遷移する
	request = enAthleticStateType_Move;
	return true;
}

// --------------------------------------------------------
// Move State
// --------------------------------------------------------
void AthleticMoveState::Enter()
{
	// 初期方向をセット
	m_isMovingUp = true;
}

void AthleticMoveState::Update()
{
	if (!m_owner) return;

	float deltaTime = g_gameTime->GetFrameDeltaTime();

	// Stageから設定値を取得
	float moveSpeed = m_owner->GetMoveSpeed() * deltaTime;
	float maxY = m_owner->GetMaxY();
	float minY = m_owner->GetMinY();

	// 次の座標を計算
	Vector3 nextPosition = ComputeNextPosition(
		m_owner->GetPosition(),
		maxY,
		minY,
		moveSpeed,
		m_isMovingUp
	);

	// 座標更新
	m_owner->SetPosition(nextPosition);
}

void AthleticMoveState::Exit()
{
}

bool AthleticMoveState::RequestState(int& request)
{
	// ずっと動き続けるなら遷移しない
	return false;
}