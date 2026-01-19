/**
 * @file NoobEnemy.h
 * @brief 雑魚敵（NoobEnemy）のクラス定義
 */
#pragma once
#include "EnemyBase.h"
#include "EnemyStatePattern.h"


 /**
  * @class NoobEnemy
  * @brief 雑魚敵（巡回型エネミー）の実装クラス
  * @note EnemyBaseを継承しています。
  * このクラス自体は「体」としての役割が強く、移動ルートや判断などの「頭脳」は
  * EnemyControllerクラスが担当します。
  */
class NoobEnemy : public EnemyBase
{
	// NOTE:  NoobEnemy用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	// これにより、ステート側から移動ベクトル(m_moveVector)などを直接参照可能にします。
	friend class NoobEnemyIdleState;
	friend class NoobEnemyMoveState;


public:
	/**
	 * @brief コンストラクタ
	 */
	NoobEnemy();

	/**
	 * @brief デストラクタ
	 */
	virtual ~NoobEnemy();

	/**
	 * @brief 初期化処理
	 * @details モデルロード、ステート生成、物理・コリジョンの設定を行います。
	 * @return bool 成功時true
	 */
	virtual bool Start() override;

	/**
	 * @brief 更新処理
	 * @details ステートの更新と、モデル・物理座標の同期を行います。
	 */
	virtual void Update() override;

	/**
	 * @brief 描画処理
	 * @param[in,out] rc 描画コンテキスト
	 */
	virtual void Render(RenderContext& rc);


public:
	/**
	 * @brief ステータスを取得する
	 * @return NoobEnemyStatus* 雑魚敵用ステータスへのポインタ
	 */
	NoobEnemyStatus* GetStatus() { return dynamic_cast<NoobEnemyStatus*>(m_status); }

	/**
	 * @brief 移動方向ベクトルを設定する
	 * @param[in] moveVector 正規化された移動方向
	 * @note EnemyController（AI）から毎フレーム設定されます。
	 */
	void SetMoveVector(const Vector3& moveVector) { m_moveVector = moveVector; }

	/**
	 * @brief 移動力を設定する
	 * @param[in] power 移動係数（0.0fなら停止、1.0fなら移動）
	 * @note EnemyController（AI）から設定されます。
	 */
	void SetMovePower(const float power) { m_movePower = power; }



private:
	/** 現在のステートID */
	EnNoobEnemyStateType m_currentState = enNoobEnemyStateType_Idle;

	// NOTE:雑魚敵の状態の箱を作る
	// 配列の中をEnNoobEnemyStateType_MaxにすることでEnNoobEnemyStateTypeの中に状態を追加するだけで箱の数を追加できる
	/** ステートインスタンスのリスト */
	INoobEnemyState* m_stateList[EnNoobEnemyStateType_Max];

	/** 移動方向（AIから与えられる） */
	Vector3 m_moveVector = Vector3::Zero;

	/** 移動力（AIから与えられる） */
	float m_movePower = 0.0f;
};