/**
 * @file AthleticState.h
 * @brief アスレチック（足場）の挙動を定義するステートクラス群定義
 */
#pragma once

class AthleticStage;

/**
 * @enum EnAthleticStateType
 * @brief アスレチックのステート（状態）の種類
 */
enum EnAthleticStateType
{
	enAthleticStateType_Idle, //!< 待機状態
	enAthleticStateType_Move, //!< 上下移動状態
	EnAthleticStateType_Max,  //!< ステートの総数
};

/**
 * @class IAthleticState
 * @brief ステートパターンの基底インターフェース
 * @note すべてのアスレチック用ステートはこのインターフェースを実装します。
 */
class IAthleticState
{
public:
	IAthleticState() {}
	virtual ~IAthleticState() {}

	/**
	 * @brief ステート開始時に呼ばれる処理
	 */
	virtual void Enter() = 0;

	/**
	 * @brief 毎フレーム呼ばれる更新処理
	 */
	virtual void Update() = 0;

	/**
	 * @brief ステート終了時に呼ばれる処理
	 */
	virtual void Exit() = 0;

	/**
	 * @brief 次のステートへの遷移リクエストを確認する
	 * @param[out] request 遷移先のステートIDを格納する変数
	 * @return bool 遷移が必要な場合はtrue、維持する場合はfalse
	 */
	virtual bool RequestState(int& request) = 0;
};

/**
 * @class AthleticStateBase
 * @brief アスレチック用ステートの共通基底クラス
 * @details 所有者（AthleticStage）へのポインタを保持し、共通の振る舞いを提供します。
 */
class AthleticStateBase : public IAthleticState
{
public:
	/**
	 * @brief コンストラクタ
	 * @param[in] owner ステートを所有するAthleticStageインスタンス
	 */
	AthleticStateBase(AthleticStage* owner) : m_owner(owner) {}
	virtual ~AthleticStateBase() {}

	virtual void Enter() override {}
	virtual void Update() override {}
	virtual void Exit() override {}

	/**
	 * @brief デフォルトでは遷移しない（false）を返す
	 */
	virtual bool RequestState(int& request) override { return false; }

protected:
	/** このステートを管理しているAthleticStageオブジェクト */
	AthleticStage* m_owner = nullptr;
};

/**
 * @class AthleticIdleState
 * @brief 待機ステート
 * @details 現在の実装では、即座に移動ステートへ遷移します。
 */
class AthleticIdleState : public AthleticStateBase
{
public:
	AthleticIdleState(AthleticStage* owner) : AthleticStateBase(owner) {}
	~AthleticIdleState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};

/**
 * @class AthleticMoveState
 * @brief 移動ステート（上下反復）
 * @details 設定された範囲内で上下に移動し続けます。
 */
class AthleticMoveState : public AthleticStateBase
{
public:
	AthleticMoveState(AthleticStage* owner) : AthleticStateBase(owner) {}
	~AthleticMoveState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;

private:
	/** 上に動いているかどうかのフラグ（true:上昇中, false:下降中） */
	bool m_isMovingUp = true;
};