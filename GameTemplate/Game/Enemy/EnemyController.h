/**
 * @file EnemyController.h
 * @brief 敵キャラクター（NoobEnemy）のAI制御クラス定義
 * @author (あなたの名前)
 */
#pragma once

class NoobEnemy;

/**
 * @class EnemyController
 * @brief 敵キャラクターの思考・制御を行うAIクラス
 * @note IGameObjectを継承していますが、描画は行わず、保持している NoobEnemy インスタンスを操作します。
 * 関数ポインタを利用したステートマシンにより、待機・移動・目的地決定などの行動を切り替えます。
 */
class EnemyController : public IGameObject
{
	// 関数ポインタの型定義
	using EnterFunc = void(*)(EnemyController*);
	using UpdateFunc = void(*)(EnemyController*);
	using ExitFunc = void(*)(EnemyController*);
	using CheckFunc = bool(*)(EnemyController*);

private:
	/**
	 * @struct StateFunc
	 * @brief 1つのステート（状態）における振る舞いをまとめた構造体
	 */
	struct StateFunc
	{
		EnterFunc m_enter;  //!< ステート開始時処理
		UpdateFunc m_update;//!< 更新処理
		ExitFunc m_exit;    //!< ステート終了時処理
		CheckFunc m_check;  //!< 遷移判定処理（trueを返すと次のステートへ遷移）
	};


private:
	/**
	 * @enum EnAIType
	 * @brief AIのステート種類
	 */
	enum EnAIType
	{
		enAIType_Idle,            //!< 待機
		enAIType_Move,            //!< 移動実行
		enAIType_SarchTargetMove, //!< 次の移動先を決定（Search）
		enAIType_Max,
		enAIType_Invalid = enAIType_Max,
	};


public:
	/**
	 * @enum EnEnemyType
	 * @brief 敵の個体タイプ（巡回ルートのパターン）
	 * @note このタイプによって、Initialize時に設定される巡回座標リストが切り替わります。
	 */
	enum EnEnemyType
	{
		enEnemyType_A, enEnemyType_B, enEnemyType_C, enEnemyType_D,
		enEnemyType_E, enEnemyType_F, enEnemyType_G, enEnemyType_H,
		enEnemyType_I, enEnemyType_J, enEnemyType_K, enEnemyType_L,
		enEnemyType_Max,
	};

public:
	/**
	 * @brief コンストラクタ
	 */
	EnemyController();

	/**
	 * @brief デストラクタ
	 */
	~EnemyController();

	/**
	 * @brief 開始処理
	 * @return 正常終了なら true
	 */
	bool Start() override;

	/**
	 * @brief 更新処理
	 * @details 現在のステートの関数を実行し、遷移条件(Check)を満たせば次のステートへ移行します。
	 */
	void Update() override;

	/**
	 * @brief 描画処理
	 * @note AI自体は不可視のため、描画処理は行いません。
	 * @param[in] rc レンダリングコンテキスト
	 */
	void Render(RenderContext& rc) override;

	/**
	 * @brief 操作対象の敵キャラクターを設定する
	 * @param[in] target 操作するNoobEnemyへのポインタ
	 */
	void SetTarget(NoobEnemy* target) { m_target = target; }

	/**
	 * @brief 敵のタイプ（巡回ルート）を設定する
	 * @param[in] type 敵のタイプID
	 */
	void SetEnemyType(EnEnemyType type);

public:
	/**
	 * @brief 指定タイプの初期出現位置を取得する
	 * @param[in] type 敵のタイプ
	 * @return 初期座標(Vector3)
	 */
	static Vector3 GetInitPosition(EnEnemyType type);

private:
	EnAIType m_currentState = enAIType_Idle;       //!< 現在の状態
	EnAIType m_requestState = enAIType_Invalid;    //!< 次に遷移したい状態

	// 操作対象となるエネミー
	NoobEnemy* m_target = nullptr;                 //!< 操作対象
	EnEnemyType m_enemyType = enEnemyType_A;       //!< 敵のタイプ
	Vector3 m_targetPosition = Vector3::Zero;      //!< 現在目指している移動目標地点
	int m_moveIndex = 0;                           //!< 巡回ルート上の現在のインデックス

	/**
	 * 状態処理周り
	 */
private:
	/** 全インスタンスで共有するステート関数のリスト */
	static std::vector<std::unique_ptr<StateFunc>> m_stateFuncList;

public:
	/**
	 * @brief AIシステムの初期化
	 * @details ステート関数の登録を行います。ゲーム開始時に一度だけ呼ぶ必要があります。
	 */
	static void Initialize();

	/**
	 * @brief ステート関数をリストに追加するヘルパー関数
	 * @param[in] enter ステート開始時の関数
	 * @param[in] update ステート更新中の関数
	 * @param[in] exit ステート終了時の関数
	 * @param[in] check ステート遷移判定関数
	 */
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
	// --- ステート関数群 ---

	/**
	 * @name Idle State (待機)
	 * @{
	 */
	static void IdleEnter(EnemyController* enemy);
	static void IdleUpdate(EnemyController* enemy);
	static void IdleExit(EnemyController* enemy);
	static bool IdleCheck(EnemyController* enemy);
	/** @} */

	/**
	 * @name Move State (移動実行)
	 * @{
	 */
	static void MoveEnter(EnemyController* enemy);
	static void MoveUpdate(EnemyController* enemy);
	static void MoveExit(EnemyController* enemy);
	static bool MoveCheck(EnemyController* enemy);
	/** @} */

	/**
	 * @name Search Move Target State (移動先決定)
	 * @{
	 */
	static void SarchMoveTargetEnter(EnemyController* enemy);
	static void SarchMoveTargetUpdate(EnemyController* enemy);
	static void SarchMoveTargetExit(EnemyController* enemy);
	static bool SarchMoveTargetCheck(EnemyController* enemy);
	/** @} */
};