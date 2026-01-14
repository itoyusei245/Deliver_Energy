#pragma once
#include "AthleticBase.h"
#include "AthleticState.h" // 追加
#include <string>

class AthleticStage : public AthleticBase
{
	// ステートクラスからアクセスできるようにする
	friend class AthleticIdleState;
	friend class AthleticMoveState;

public:
	AthleticStage();
	~AthleticStage();

	bool Start() override;
	void Update() override; // Updateを追加

	// 位置の設定
	void SetPosition(const Vector3& pos) { m_position = pos; }
	const Vector3& GetPosition() const { return m_position; }

	// モデルパス設定
	void SetModelPath(const char* path) { m_modelPath = path; }

	// 移動パラメータの設定用関数
	void SetMoveSettings(float speed, float range) {
		m_moveSpeed = speed;
		m_moveRange = range;
	}

	// ステート側からパラメータを取得するためのゲッター
	float GetMoveSpeed() const { return m_moveSpeed; }
	float GetMaxY() const { return m_defaultPosition.y + m_moveRange; }
	float GetMinY() const { return m_defaultPosition.y - m_moveRange; }

private:
	Vector3 m_position = Vector3::Zero;
	std::string m_modelPath = "Assets/modelData/Stage/map_Athletic.tkm";

	// --- ステートパターン用変数 ---
	EnAthleticStateType m_currentState = enAthleticStateType_Idle;
	IAthleticState* m_stateList[EnAthleticStateType_Max];

	// --- 移動用パラメータ ---
	Vector3 m_defaultPosition = Vector3::Zero; // 基準位置
	float m_moveSpeed = 0.0f; // 移動速度
	float m_moveRange = 0.0f; // 上下移動の振れ幅
};