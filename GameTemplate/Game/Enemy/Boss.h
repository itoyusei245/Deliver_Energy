/**
 * @file Boss.h
 * @brief ボスキャラクターのクラス定義
 */
#pragma once
#include "EnemyBase.h"
#include "EnemyStatePattern.h"

 /**
  * @class Boss
  * @brief ボスキャラクターの実装クラス
  * @note EnemyBaseを継承しています。
  * ステートパターンを用いて行動（待機、移動、眷属生成など）を制御します。
  */
class Boss : public EnemyBase
{
	// NOTE: Boss用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	// これにより、ステートクラス側からBossのprivateメンバ（座標やフラグなど）を直接操作可能にする。
	friend class BossIdleState;
	friend class BossMoveState;
	friend class BossCreateFamiliarState;

public:
	/**
	 * @brief コンストラクタ
	 */
	Boss();

	/**
	 * @brief デストラクタ
	 */
	virtual ~Boss();

	/**
	 * @brief 初期化処理
	 * @details モデル読み込み、物理・コリジョンの設定、ステートの生成を行います。
	 * @return bool 初期化成功ならtrue
	 */
	virtual bool Start() override;

	/**
	 * @brief 更新処理
	 * @details ステートの更新と、座標の同期を行います。
	 */
	virtual void Update() override;

	/**
	 * @brief 描画処理
	 * @param[in,out] rc 描画コンテキスト
	 */
	virtual void Render(RenderContext& rc);

public:
	/**
	 * @brief ボス用ステータスを取得する
	 * @details 基底クラスのm_statusをBossStatus型にキャストして返します。
	 * @return BossStatus* ボスのステータス情報
	 */
	BossStatus* GetStatus() { return dynamic_cast<BossStatus*>(m_status); }

	/**
	 * @brief 眷属（Familiar）生成リクエストフラグを設定する
	 * @param[in] flg trueなら生成をリクエスト
	 */
	void SetRequestCreateFamiliar(const bool flg) { m_isRequestCreateFamiliar = flg; }

	/**
	 * @brief 眷属生成リクエスト中かどうかを取得する
	 * @return bool 生成リクエスト中ならtrue
	 */
	bool IsRequestCreateFamiliar() const { return m_isRequestCreateFamiliar; }

	/**
	 * @brief 眷属が生成済みかどうかを設定する
	 * @param[in] flg 生成済みならtrue
	 */
	void SetCreatedFamiliar(const bool flg) { m_isCreatedFamiliar = flg; }

	/**
	 * @brief 眷属が生成済みかどうかを取得する
	 * @return bool 生成済みならtrue
	 */
	bool IsCreatedFamiliar() const { return m_isCreatedFamiliar; }

public:
	/**
	 * @brief 上昇中かどうかを取得する（移動ステート用）
	 */
	bool IsMovingUp()const { return m_isMovingUp; }

	/**
	 * @brief 移動方向（上昇/下降）を設定する
	 * @param[in] flg true:上昇, false:下降
	 */
	void SetMovingUp(bool flg) { m_isMovingUp = flg; }

private:
	/** 移動方向フラグ（true:上昇中, false:下降中） */
	bool m_isMovingUp = true;

private:
	/** 現在のステートID */
	EnBossStateType m_currentState = enBossStateType_Idle;

	// NOTE:ボスの状態の箱を作る
	// 配列の中をEnBossStateType_MaxにすることでEnBossStateTypeの中に状態を追加するだけで箱の数を追加できる
	/**
	 * @brief ステートインスタンスのリスト
	 * @details ステートごとに作ったクラスの情報を保持します。
	 */
	IBossState* m_stateList[EnBossStateType_Max];

	/** 眷属の生成をリクエストするフラグ */
	bool m_isRequestCreateFamiliar = false;
	/** 眷属生成済みか判定するフラグ */
	bool m_isCreatedFamiliar = false;
};