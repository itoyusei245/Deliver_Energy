/**
 * @file EnemyBase.h
 * @brief 敵キャラクターの基底クラス定義
 */
#pragma once
#include "EnemyStatus.h"

 /**
  * @class EnemyBase
  * @brief 敵キャラクターの共通基底クラス
  * @note 新しい敵キャラクターを作成する際は、必ずこのクラスを継承してください。
  * 座標管理、モデル描画、物理判定、ステータス管理の基本機能を提供します。
  */
class EnemyBase : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	 */
	EnemyBase() {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~EnemyBase() {}

	/**
	 * @brief 初期化処理
	 * @return bool 成功時true
	 */
	virtual bool Start() override { return true; }

	/**
	 * @brief 更新処理
	 */
	virtual void Update() override {}

	/**
	 * @brief 描画処理
	 * @param[in,out] rc 描画コンテキスト
	 */
	virtual void Render(RenderContext& rc) override {}

	// 死亡時に呼ぶ関数
	virtual void OnDead();

	/**
	 * @brief 座標を設定する
	 * @param[in] position 設定する座標
	 */
	void SetPosition(const Vector3& position) { m_position = position; }

	/**
	 * @brief 現在の座標を取得する
	 * @return const Vector3& 現在の座標
	 */
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief 初期座標（スポーン位置）を設定する
	 * @param[in] position 初期座標
	 */
	void SetDefaultPosition(const Vector3& position) { m_defaultPosition = position; }

	/**
	 * @brief 初期座標（スポーン位置）を取得する
	 * @return const Vector3& 初期座標
	 */
	const Vector3& GetDefaultPosition() const { return m_defaultPosition; }

	/**
	 * @brief スケール（大きさ）を設定する
	 * @param[in] scale 設定するスケール値
	 */
	void SetScale(const Vector3& scale) { m_scale = scale; }

	/**
	 * @brief 現在のスケールを取得する
	 * @return const Vector3& 現在のスケール
	 */
	const Vector3& GetScale() const { return  m_scale; }

	/**
	 * @brief 回転を設定する
	 * @param[in] rotation 設定するクォータニオン
	 */
	void SetRotation(const Quaternion& rotation) { m_rotation = rotation; }

	/**
	 * @brief 現在の回転を取得する
	 * @return const Quaternion& 現在の回転
	 */
	const Quaternion& GetRotation() const { return m_rotation; }

	/**
	 * @brief 当たり判定オブジェクトを取得する
	 * @return CollisionObject* コリジョンオブジェクトへのポインタ
	 */
	CollisionObject* GetCollisionObject() { return &m_collisionObject; }


protected:
	/** 現在の座標 */
	Vector3 m_position = Vector3::Zero;
	/** 現在のスケール */
	Vector3 m_scale = Vector3::One;
	/** 初期配置座標（リスポーン時などに使用想定） */
	Vector3 m_defaultPosition = Vector3::Zero;

	/** 現在の回転 */
	Quaternion m_rotation = Quaternion::Identity;

	/** 3Dモデル描画用オブジェクト */
	ModelRender m_modelRender;

	CollisionObject m_collisionObject;

	/** * @brief 敵のステータス情報（HP、攻撃力など）
	 * @note 派生クラスで具体的なStatusクラス（BossStatusなど）をインスタンス化して保持させます。
	 */
	IEnemyStatus* m_status = nullptr;

	/** * @brief 物理挙動用オブジェクト（静的）
	 */
	PhysicsStaticObject m_physicsStaticObject;
};