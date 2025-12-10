/**
 * EnemyBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once


class Boss;
class Familiar;
class NoobEnemy;

class IEnemyState : public Noncopyable
{
public:
	IEnemyState() {}
	virtual ~IEnemyState() {}

	/** 初期化用 */
	virtual void Eneter() = 0;
	/** 更新用 */
	virtual void Update() = 0;
	/** 後処理用 */
	virtual void Exit() = 0;
	/** 状態切り替えリクエスト */
	virtual bool RequestState(int& request) = 0;
};




/*****************************************************************************/


/**列挙型：上から書いたものに数字を振り分ける */
enum EnBossStateType
{
	enBossStateType_Idle,
	enBossStateType_Move,
	enBossStateType_CreateFamiliar,
	EnBossStateType_Max,
};




class IBossState : public IEnemyState
{
public:
	IBossState(Boss* owner) : m_owner(owner) {}
	virtual ~IBossState() {}

	/** 初期化用 */
	virtual void Eneter() override {}
	/** 更新用 */
	virtual void Update() override {}
	/** 後処理用 */
	virtual void Exit() override {}
	/** 状態切り替えリクエスト */
	virtual bool RequestState(int& request) override { return false; }


protected:
	Boss* m_owner = nullptr;
};




class BossIdleState : public IBossState
{
public:
	BossIdleState(Boss* owner) : IBossState(owner) {}
	~BossIdleState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};




class BossMoveState : public IBossState
{
public:
	BossMoveState(Boss* owner) : IBossState(owner) {}
	~BossMoveState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;


private:
	bool m_isMovingUp = true;
};




class BossCreateFamiliarState : public IBossState
{
public:
	BossCreateFamiliarState(Boss* owner) : IBossState(owner) {}
	~BossCreateFamiliarState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};




/*****************************************************************************/


enum EnFamiliarStateType
{
	enFamiliarStateType_Idle,
	enFamiliarStateType_Move,
	EnFamiliarStateType_Max,
};




class IFamiliarState : public IEnemyState
{
public:
	IFamiliarState(Familiar* owner) : m_owner(owner) {}
	virtual ~IFamiliarState() {}

	/** 初期化用 */
	virtual void Eneter() override {}
	/** 更新用 */
	virtual void Update() override {}
	/** 後処理用 */
	virtual void Exit() override {}
	/** 状態切り替えリクエスト */
	virtual bool RequestState(int& request) override { return false; }


protected:
	Familiar* m_owner = nullptr;
};




class FamiliarIdleState : public IFamiliarState
{
public:
	FamiliarIdleState(Familiar* owner) : IFamiliarState(owner) {}
	~FamiliarIdleState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};




class FamiliarMoveState : public IFamiliarState
{
public:
	FamiliarMoveState(Familiar* owner) : IFamiliarState(owner) {}
	~FamiliarMoveState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;


private:
	bool m_isMovingUp = true;
};




/*****************************************************************************/


enum EnNoobEnemyStateType
{
	enNoobEnemyStateType_Idle,
	enNoobEnemyStateType_Move,
	EnNoobEnemyStateType_Max,
};




class INoobEnemyState : public IEnemyState
{
public:
	INoobEnemyState(NoobEnemy* owner) : m_owner(owner) {}
	virtual ~INoobEnemyState() {}

	/** 初期化用 */
	virtual void Eneter() override {}
	/** 更新用 */
	virtual void Update() override {}
	/** 後処理用 */
	virtual void Exit() override {}
	/** 状態切り替えリクエスト */
	virtual bool RequestState(int& request) override { return false; }


protected:
	NoobEnemy* m_owner = nullptr;
};




class NoobEnemyIdleState : public INoobEnemyState
{
public:
	NoobEnemyIdleState(NoobEnemy* owner) : INoobEnemyState(owner) {}
	~NoobEnemyIdleState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;
};




class NoobEnemyMoveState : public INoobEnemyState
{
public:
	NoobEnemyMoveState(NoobEnemy* owner) : INoobEnemyState(owner) {}
	~NoobEnemyMoveState() {}


	void Eneter() override;
	void Update() override;
	void Exit() override;
	bool RequestState(int& request) override;


};
