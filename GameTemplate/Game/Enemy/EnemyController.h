/**
 * EnemyBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once


class NoobEnemy;


/**
 * 敵キャラクターのAI
 * NOTE:プレイヤーのコントローラー操作を自動でするようなイメージ
 */
class EnemyController : public IGameObject
{
	using EnterFunc = void(*)(EnemyController*);
	using UpdateFunc = void(*)(EnemyController*);
	using ExitFunc = void(*)(EnemyController*);
	using CheckFunc = bool(*)(EnemyController*);

private:
	struct StateFunc
	{
		EnterFunc m_enter;
		UpdateFunc m_update;
		ExitFunc m_exit;
		CheckFunc m_check;
	};


private:
	enum EnAIType
	{
		enAIType_Idle,
		enAIType_Move,
		enAIType_SarchTargetMove,
		enAIType_Max,
		enAIType_Invalid = enAIType_Max,
	};


public:
	/** 生まれた場所によって変わるはず */
	enum EnEnemyType
	{
		enEnemyType_A,enEnemyType_B,enEnemyType_C,enEnemyType_D,
		enEnemyType_E,enEnemyType_F,enEnemyType_G,enEnemyType_H,
		enEnemyType_I,enEnemyType_J,enEnemyType_K,enEnemyType_L,
		enEnemyType_Max,
	};



public:
	EnemyController();
	~EnemyController();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void SetTarget(NoobEnemy* target) { m_target = target; }
	void SetEnemyType(EnEnemyType type);

public:
	static Vector3 GetInitPosition(EnEnemyType type);

private:
	EnAIType m_currentState = enAIType_Idle;
	EnAIType m_requestState = enAIType_Invalid;

	// 操作対象となるエネミー
	NoobEnemy* m_target = nullptr;
	EnEnemyType m_enemyType = enEnemyType_A;
	Vector3 m_targetPosition = Vector3::Zero;
	int m_moveIndex = 0;



	/**
	 * 状態処理周り
	 */
private:
	static std::vector<std::unique_ptr<StateFunc>> m_stateFuncList;

public:
	static void Initialize();
	static void AddStateFunc(EnterFunc enter, UpdateFunc update, ExitFunc exit, CheckFunc check)
	{
		auto stateFunc = std::make_unique<StateFunc>();
		stateFunc->m_enter = enter;
		stateFunc->m_update = update;
		stateFunc->m_exit = exit;
		stateFunc->m_check = check;
		m_stateFuncList.push_back(std::move(stateFunc));
	}


private:
	/**
	 * 待機
	 */
	static void IdleEnter(EnemyController* enemy);
	static void IdleUpdate(EnemyController* enemy);
	static void IdleExit(EnemyController* enemy);
	static bool IdleCheck(EnemyController* enemy);


	/**
	 * 指定した移動先へ移動
	 */
	static void MoveEnter(EnemyController* enemy);
	static void MoveUpdate(EnemyController* enemy);
	static void MoveExit(EnemyController* enemy);
	static bool MoveCheck(EnemyController* enemy);

	/**
	 * 移動先を考える関連
	 */
	static void SarchMoveTargetEnter(EnemyController* enemy);
	static void SarchMoveTargetUpdate(EnemyController* enemy);
	static void SarchMoveTargetExit(EnemyController* enemy);
	static bool SarchMoveTargetCheck(EnemyController* enemy);
};

