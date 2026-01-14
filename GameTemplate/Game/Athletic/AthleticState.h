#pragma once

class AthleticStage;

// ステートの種類の列挙
enum EnAthleticStateType
{
	enAthleticStateType_Idle, // 待機
	enAthleticStateType_Move, // 上下移動
	EnAthleticStateType_Max,
};

// 基底インターフェース
class IAthleticState
{
public:
	IAthleticState() {}
	virtual ~IAthleticState() {}

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual bool RequestState(int& request) = 0;
};

// Athletic用基底クラス
class AthleticStateBase : public IAthleticState
{
public:
	AthleticStateBase(AthleticStage* owner) : m_owner(owner) {}
	virtual ~AthleticStateBase() {}

	virtual void Enter() override {}
	virtual void Update() override {}
	virtual void Exit() override {}
	virtual bool RequestState(int& request) override { return false; }

protected:
	AthleticStage* m_owner = nullptr;
};

// 待機ステート
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

// 移動ステート（上下）
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
	bool m_isMovingUp = true; // 上に動いているかどうかのフラグ
};