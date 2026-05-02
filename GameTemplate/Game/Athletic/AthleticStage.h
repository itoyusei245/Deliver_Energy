/**
 * @file AthleticStage.h
 * @brief アスレチックステージ（動く足場など）の実装クラス定義
 */
#pragma once
#include "AthleticBase.h"
#include "AthleticState.h"
#include <string>

 /**
  * @class AthleticStage
  * @brief 具体的なアスレチックステージ（足場）クラス
  * @note ステートパターンを用いて「待機」「移動」などの振る舞いを切り替えます。
  * また、物理判定(PhysicsStaticObject)と描画モデル(ModelRender)の位置同期も行います。
  */
class AthleticStage : public AthleticBase
{
	// ステートクラスからこのクラスのprivateメンバ（座標など）へアクセスできるようにする
	friend class AthleticIdleState;
	friend class AthleticMoveState;

public:
	/**
	 * @brief コンストラクタ
	 */
	AthleticStage();

	/**
	 * @brief デストラクタ
	 * @details 確保したステートのメモリを解放します。
	 */
	~AthleticStage();

	/**
	 * @brief 初期化処理
	 * @details モデルのロード、物理情報の作成、初期ステートの構築を行います。
	 * @return bool 初期化成功ならtrue
	 */
	bool Start() override;

	/**
	 * @brief 更新処理
	 * @details ステートの更新と、それに伴う座標の同期（モデル・物理）を行います。
	 */
	void Update() override; // Updateを追加

	/**
	 * @brief 現在位置を設定する
	 * @param[in] pos 設定する座標
	 */
	void SetPosition(const Vector3& pos) { m_position = pos; }

	/**
	 * @brief 現在位置を取得する
	 * @return const Vector3& 現在の座標
	 */
	const Vector3& GetPosition() const { return m_position; }

	/**
	 * @brief ロードするモデルのファイルパスを設定する
	 * @param[in] path tkmファイルのパス
	 */
	void SetModelPath(const char* path) { m_modelPath = path; }

	/**
	 * @brief 移動ギミックのパラメータを設定する
	 * @param[in] speed 移動速度
	 * @param[in] range 上下移動の振れ幅（基準位置からの距離）
	 */
	void SetMoveSettings(float speed, float range) {
		m_moveSpeed = speed;
		m_moveRange = range;
	}

	// --- ステート側からパラメータを取得するためのゲッター ---

	/**
	 * @brief 移動速度を取得
	 */
	float GetMoveSpeed() const { return m_moveSpeed; }

	/**
	 * @brief 移動範囲の最大Y座標を取得
	 */
	float GetMaxY() const { return m_defaultPosition.y + m_moveRange; }

	/**
	 * @brief 移動範囲の最小Y座標を取得
	 */
	float GetMinY() const { return m_defaultPosition.y - m_moveRange; }

private:
	static constexpr const char* DEFAULT_MODEL_PATH = "Assets/modelData/Stage/map_Athletic.tkm";

	Vector3 m_position = Vector3::Zero;
	std::string m_modelPath = DEFAULT_MODEL_PATH;

	// --- ステートパターン用変数 ---
	/** 現在のステートID */
	EnAthleticStateType m_currentState = enAthleticStateType_Idle;
	/** ステートクラスのインスタンス配列 */
	IAthleticState* m_stateList[EnAthleticStateType_Max];

	// --- 移動用パラメータ ---
	/** 基準位置（初期位置） */
	Vector3 m_defaultPosition = Vector3::Zero;
	/** 移動速度 */
	float m_moveSpeed = 0.0f;
	/** 上下移動の振れ幅 */
	float m_moveRange = 0.0f;
};