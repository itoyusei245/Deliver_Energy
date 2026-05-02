/**
 * @file EnemyStatePattern.h
 * @brief 敵キャラクターの各ステート（状態）クラスの定義
 */
#pragma once

class Boss;
class Familiar;
class NoobEnemy;

/**
 * @class IEnemyState
 * @brief 敵ステートの基底インターフェース
 * @note 全ての敵の全てのステートは、このインターフェースを実装します。
 */
class IEnemyState : public Noncopyable
{
public:
	IEnemyState() {}
	virtual ~IEnemyState() {}

	/** * @brief ステート開始時処理（初期化）
	 * @note 状態遷移してきた直後に1度だけ呼ばれます。
	 */
	virtual void Enter() = 0;

	/** * @brief ステート更新処理
	 * @note 毎フレーム呼び出されます。
	 */
	virtual void Update() = 0;

	/** * @brief ステート終了時処理（後処理）
	 * @note 別の状態へ遷移する直前に1度だけ呼ばれます。
	 */
	virtual void Exit() = 0;

	/** * @brief 状態切り替えリクエスト確認
	 * @param[out] request 次に遷移すべきステートIDを格納
	 * @return bool 遷移が必要な場合はtrue
	 */
	virtual bool RequestState(int& request) = 0;
};


/*****************************************************************************/
// Boss States
/*****************************************************************************/

/**
 * @enum EnBossStateType
 * @brief ボスの状態の種類
 */
enum EnBossStateType
{
	enBossStateType_Idle,           //!< 待機
	enBossStateType_Move,           //!< 上下移動
	enBossStateType_CreateFamiliar, //!< 眷属生成（攻撃フェーズ）
	EnBossStateType_Max,
};


/**
 * @class IBossState
 * @brief ボス用ステートの基底クラス
 * @note Bossクラスへのポインタ(m_owner)を保持し、ボス固有の操作を行えるようにします。
 */
class IBossState : public IEnemyState
{
public:
	IBossState(Boss* owner) : m_owner(owner) {}
	virtual ~IBossState() {}

	virtual void Enter() override {}
	virtual void Update() override {}
	virtual void Exit() override {}
	virtual bool RequestState(int& request) override { return false; }

protected:
	/** このステートを所有するBossインスタンス */
	Boss* m_owner = nullptr;
};


/**
 * @class BossIdleState
 * @brief ボスの待機ステート
 */
class BossIdleState : public IBossState
{
public:
	BossIdleState(Boss* owner) : IBossState(owner) {}
	~BossIdleState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};


/**
 * @class BossMoveState
 * @brief ボスの移動ステート
 * @details 上下に反復移動を行います。
 */
class BossMoveState : public IBossState
{
public:
	BossMoveState(Boss* owner) : IBossState(owner) {}
	~BossMoveState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;

private:
	bool m_isMovingUp = true;
};


/**
 * @class BossCreateFamiliarState
 * @brief ボスの眷属生成ステート
 * @details EnemyManagerに対して眷属（Familiar）の生成をリクエストし、完了を待ちます。
 */
class BossCreateFamiliarState : public IBossState
{
public:
	BossCreateFamiliarState(Boss* owner) : IBossState(owner) {}
	~BossCreateFamiliarState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};


/*****************************************************************************/
// Familiar States
/*****************************************************************************/

/**
 * @enum EnFamiliarStateType
 * @brief 眷属（Familiar）の状態の種類
 */
enum EnFamiliarStateType
{
	enFamiliarStateType_Idle, //!< 待機
	enFamiliarStateType_Move, //!< 移動（ボスに追従）
	EnFamiliarStateType_Max,
};


/**
 * @class IFamiliarState
 * @brief 眷属用ステートの基底クラス
 */
class IFamiliarState : public IEnemyState
{
public:
	IFamiliarState(Familiar* owner) : m_owner(owner) {}
	virtual ~IFamiliarState() {}

	virtual void Enter() override {}
	virtual void Update() override {}
	virtual void Exit() override {}
	virtual bool RequestState(int& request) override { return false; }

protected:
	Familiar* m_owner = nullptr;
};


/**
 * @class FamiliarIdleState
 * @brief 眷属の待機ステート
 */
class FamiliarIdleState : public IFamiliarState
{
public:
	FamiliarIdleState(Familiar* owner) : IFamiliarState(owner) {}
	~FamiliarIdleState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};


/**
 * @class FamiliarMoveState
 * @brief 眷属の移動ステート
 * @details ボスの動きに合わせて上下移動を行います。
 */
class FamiliarMoveState : public IFamiliarState
{
public:
	FamiliarMoveState(Familiar* owner) : IFamiliarState(owner) {}
	~FamiliarMoveState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;

private:
	bool m_isMovingUp = true;
};


/*****************************************************************************/
// NoobEnemy States
/*****************************************************************************/

/**
 * @enum EnNoobEnemyStateType
 * @brief 雑魚敵（NoobEnemy）の状態の種類
 */
enum EnNoobEnemyStateType
{
	enNoobEnemyStateType_Idle, //!< 待機
	enNoobEnemyStateType_Move, //!< 移動（AIControllerによる制御）
	EnNoobEnemyStateType_Max,
};


/**
 * @class INoobEnemyState
 * @brief 雑魚敵用ステートの基底クラス
 */
class INoobEnemyState : public IEnemyState
{
public:
	INoobEnemyState(NoobEnemy* owner) : m_owner(owner) {}
	virtual ~INoobEnemyState() {}

	virtual void Enter() override {}
	virtual void Update() override {}
	virtual void Exit() override {}
	virtual bool RequestState(int& request) override { return false; }

protected:
	NoobEnemy* m_owner = nullptr;
};


/**
 * @class NoobEnemyIdleState
 * @brief 雑魚敵の待機ステート
 */
class NoobEnemyIdleState : public INoobEnemyState
{
public:
	NoobEnemyIdleState(NoobEnemy* owner) : INoobEnemyState(owner) {}
	~NoobEnemyIdleState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};


/**
 * @class NoobEnemyMoveState
 * @brief 雑魚敵の移動ステート
 */
class NoobEnemyMoveState : public INoobEnemyState
{
public:
	NoobEnemyMoveState(NoobEnemy* owner) : INoobEnemyState(owner) {}
	~NoobEnemyMoveState() {}

	void Enter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};