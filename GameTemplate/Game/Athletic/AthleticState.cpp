/**
 * @file AthleticState.cpp
 * @brief アスレチックステートの実装
 */
#include "stdafx.h"
#include "AthleticState.h"
#include "AthleticStage.h"

namespace
{
	/**
	 * @brief 上下移動における次の位置を計算するヘルパー関数
	 * @details 上限・下限に達した場合、移動方向フラグを反転させます。
	 * * @param[in] position 現在の座標
	 * @param[in] maxY 移動範囲の最大Y座標
	 * @param[in] minY 移動範囲の最小Y座標
	 * @param[in] moveSpeed フレームごとの移動量
	 * @param[in,out] isMovingUp 現在上昇中かどうかのフラグ。反転時に書き換えられます。
	 * @return Vector3 計算後の新しい座標
	 */
	Vector3 ComputeNextPosition(const Vector3& position, const float maxY, const float minY, const float moveSpeed, bool& isMovingUp)
	{
		Vector3 nextPosition = position;

		// 上昇中ならプラス、下降中ならマイナス方向に移動
		nextPosition.y += isMovingUp ? moveSpeed : -moveSpeed;

		// 上限・下限チェックと反転処理
		if (isMovingUp) {
			// 上限を超えたら位置を補正し、下降へ切り替え
			if (nextPosition.y >= maxY) {
				nextPosition.y = maxY;
				isMovingUp = false;
			}
		}
		else {
			// 下限を超えたら位置を補正し、上昇へ切り替え
			if (nextPosition.y <= minY) {
				nextPosition.y = minY;
				isMovingUp = true;
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

/**
 * @brief 遷移リクエスト
 * @details 即座に移動ステートへの遷移をリクエストします。
 */
bool AthleticIdleState::RequestState(int& request)
{
	// すぐに移動ステートへ遷移する
	request = enAthleticStateType_Move;
	return true;
}

// --------------------------------------------------------
// Move State
// --------------------------------------------------------
/**
 * @brief 移動開始時の初期化
 * @details 移動方向を「上昇」にリセットします。
 */
void AthleticMoveState::Enter()
{
	// 初期方向をセット
	m_isMovingUp = true;
}

/**
 * @brief 移動更新処理
 * @details デルタタイムを考慮して移動量を計算し、オーナーの位置を更新します。
 */
void AthleticMoveState::Update()
{
	if (!m_owner) return;

	// フレーム間の経過時間を取得
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	// Stageクラスから移動設定値を取得（速度にデルタタイムを掛けてフレームレート依存を防ぐ）
	float moveSpeed = m_owner->GetMoveSpeed() * deltaTime;
	float maxY = m_owner->GetMaxY();
	float minY = m_owner->GetMinY();

	// ヘルパー関数を使って次の座標を計算
	Vector3 nextPosition = ComputeNextPosition(
		m_owner->GetPosition(),
		maxY,
		minY,
		moveSpeed,
		m_isMovingUp
	);

	// 計算結果をオーナー（AthleticStage）に反映
	m_owner->SetPosition(nextPosition);
}

void AthleticMoveState::Exit()
{
}

/**
 * @brief 遷移リクエスト
 * @details 移動ステートは継続するため、遷移リクエストは行わない。
 */
bool AthleticMoveState::RequestState(int& request)
{
	// ずっと動き続けるなら遷移しない
	return false;
}