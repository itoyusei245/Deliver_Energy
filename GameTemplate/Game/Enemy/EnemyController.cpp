/**
 * @file EnemyController.cpp
 * @brief AI制御の実装
 */
#include "stdafx.h"
#include "EnemyController.h"
#include "Enemy/NoobEnemy.h"
#include "Enemy/EnemyManager.h" 
#include "Enemy/Boss.h"         

namespace
{
	const int MOVE_POSITON_NUM = 4; //!< 巡回ポイントの数

	/**
	 * @struct MovePositionInfo
	 * @brief 敵の巡回ルート（4つの座標）を保持する構造体
	 */
	struct MovePositionInfo
	{
		Vector3 m_target[MOVE_POSITON_NUM];

		/**
		 * @brief コンストラクタ
		 * @param a 地点1
		 * @param b 地点2
		 * @param c 地点3
		 * @param d 地点4
		 */
		MovePositionInfo(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
		{
			m_target[0] = a;
			m_target[1] = b;
			m_target[2] = c;
			m_target[3] = d;
		}
	};

	/**
	 * @brief 敵のタイプごとの巡回ルート定義リスト
	 * @note TypeA ～ TypeL までの12種類のルートが定義されています。
	 */
	static MovePositionInfo MOVE_POSITION_LIST[] = {
		MovePositionInfo(Vector3(3200.0f, -450.0f, -4000.0f), Vector3(3300.0f, -450.0f, -4000.0f), Vector3(3300.0f, -450.0f, -3900.0f), Vector3(3200.0f, -450.0f, -4000.0f)),	// TypeA
		MovePositionInfo(Vector3(3400.0f, -450.0f, -3200.0f), Vector3(3500.0f, -450.0f, -3200.0f), Vector3(3500.0f, -450.0f, -3100.0f), Vector3(3400.0f, -450.0f, -3200.0f)),	// TypeB
		MovePositionInfo(Vector3(3900.0f, -450.0f, -3300.0f), Vector3(4000.0f, -450.0f, -3300.0f), Vector3(4000.0f, -450.0f, -3400.0f), Vector3(3900.0f, -450.0f, -3500.0f)),	// TypeL
	};

	constexpr float MOVE_POWER_NORMAL = 1.0f;
	constexpr float ARRIVE_THRESHOLD  = 30.0f; 
}


// ステート関数リストの実体定義
std::vector<std::unique_ptr<EnemyController::StateFunc>> EnemyController::m_stateFuncList;


/**
 * @brief コンストラクタ
 */
EnemyController::EnemyController()
{

}


/**
 * @brief デストラクタ
 */
EnemyController::~EnemyController()
{

}


/**
 * @brief 開始処理
 */
bool EnemyController::Start()
{
	return true;
}


/**
 * @brief AIの更新
 * @details
 * 1. 現在のステートの遷移判定(Check)を実行
 * 2. 遷移する場合：現在のExit -> 次のEnter -> ステート更新
 * 3. 遷移しない場合：現在のUpdateを実行
 */
void EnemyController::Update()
{
	if (EnemyManager::GetInstance() == nullptr) return;
	// ---------------------------------------------------
	// 操作対象が消えるなら、も一緒に消滅させる
	// ---------------------------------------------------

	// ボスが死んだ場合（体が強制消滅するため、AIも自滅する）
	Boss* boss = EnemyManager::GetInstance()->GetBoss();
	if (boss != nullptr && boss->GetStatus()->IsDead()) {
		DeleteGO(this);
		return;
	}

	// プレイヤーに倒されてHPが0になった場合（10フレーム後に体が消えるため、AIを先に止める）
	if (m_target != nullptr && m_target->GetStatus() != nullptr && m_target->GetStatus()->IsDead()) {
		DeleteGO(this);
		return;
	}
	// ---------------------------------------------------

	auto& stateFunc = m_stateFuncList[m_currentState];
	if (stateFunc->m_check(this)) {
		stateFunc->m_exit(this);

		auto& nextStateFunc = m_stateFuncList[m_requestState];
		nextStateFunc->m_enter(this);

		m_currentState = m_requestState;
		return;
	}
	stateFunc->m_update(this);
}


/**
 * @brief 描画処理
 * @param rc レンダリングコンテキスト
 * @note AIは描画物を持たないため処理なし
 */
void EnemyController::Render(RenderContext& rc)
{
}

/**
 * @brief 敵のタイプ設定
 * @details タイプを設定し、最初の巡回位置へNoobEnemyを配置します。
 * @param[in] type 設定する敵タイプ
 */
void EnemyController::SetEnemyType(EnEnemyType type)
{
	m_enemyType = type;
	// NoobEnemyの位置を、ルートの最初の地点に強制移動させる
	m_target->SetPosition(MOVE_POSITION_LIST[m_enemyType].m_target[0]);
}

Vector3 EnemyController::GetInitPosition(EnEnemyType type)
{
	return MOVE_POSITION_LIST[type].m_target[0];
}

/**
 * @brief ステートマシンの初期構築
 * @details
 * - Idle (待機)
 * - Move (移動)
 */
void EnemyController::Initialize()
{
	if (!m_stateFuncList.empty()) return;
	AddStateFunc(IdleEnter, IdleUpdate, IdleExit, IdleCheck);
	AddStateFunc(MoveEnter, MoveUpdate, MoveExit, MoveCheck);
	AddStateFunc(SearchMoveTargetEnter, SearchMoveTargetUpdate, SearchMoveTargetExit, SearchMoveTargetCheck);
}

// -----------------------------------------------------------
// Idle State (待機)
// -----------------------------------------------------------

/**
 * @brief 待機ステート：開始
 */
void EnemyController::IdleEnter(EnemyController* enemy)
{
}


/**
 * @brief 待機ステート：更新
 */
void EnemyController::IdleUpdate(EnemyController* enemy)
{
}


/**
 * @brief 待機ステート：終了
 */
void EnemyController::IdleExit(EnemyController* enemy)
{
}


/**
 * @brief 待機ステート：遷移判定
 * @details 無条件で「移動先決定」へ遷移します。
 */
bool EnemyController::IdleCheck(EnemyController* enemy)
{
	// 即座に「移動先決定」ステートへ遷移
	enemy->m_requestState = enAIType_SearchTargetMove;
	return true;
}


// -----------------------------------------------------------
// Move State (移動実行)
// -----------------------------------------------------------

/**
 * @brief 移動ステート：開始
 */
void EnemyController::MoveEnter(EnemyController* enemy)
{
}


/**
 * @brief 移動ステート：更新
 * @details 目標地点へのベクトルを計算し、対象のエネミーに移動力を与えます。
 */
void EnemyController::MoveUpdate(EnemyController* enemy)
{
	Vector3 direction = enemy->m_targetPosition - enemy->m_target->GetPosition();
	direction.Normalize();

	// エネミー本体に移動ベクトルとパワーを設定
	enemy->m_target->SetMoveVector(direction);
	enemy->m_target->SetMovePower(MOVE_POWER_NORMAL);
}


/**
 * @brief 移動ステート：終了
 * @details 移動を停止するためにパワーを0にします。
 */
void EnemyController::MoveExit(EnemyController* enemy)
{
	// 移動終了のためパワーを0にする
	enemy->m_target->SetMovePower(0.0f);
}


/**
 * @brief 移動ステート：遷移判定
 * @details 目標地点との距離が30.0f未満になったら到着とみなし、次の「移動先決定」ステートへ遷移します。
 */
bool EnemyController::MoveCheck(EnemyController* enemy)
{
	Vector3 direction = enemy->m_targetPosition - enemy->m_target->GetPosition();
	if (direction.Length() < ARRIVE_THRESHOLD) {
		enemy->m_requestState = enAIType_SearchTargetMove;
		return true;
	}
	return false;
}


// -----------------------------------------------------------
// Search Move Target State (移動先決定)
// -----------------------------------------------------------

/**
 * @brief 移動先決定ステート：開始
 * @details 定義済みリストから次の座標を取得し、インデックスを進めます（ループ再生）。
 */
void EnemyController::SearchMoveTargetEnter(EnemyController* enemy)
{
	const auto& moveInfo = MOVE_POSITION_LIST[enemy->m_enemyType];
	const Vector3 targetPosition = moveInfo.m_target[enemy->m_moveIndex];

	// インデックスを進める（4点巡回したら0に戻る）
	enemy->m_moveIndex++;
	if (enemy->m_moveIndex >= MOVE_POSITON_NUM) {
		enemy->m_moveIndex = 0;
	}
	// 目標地点をメンバに保存
	enemy->m_targetPosition = targetPosition;
}


/**
 * @brief 移動先決定ステート：更新
 */
void EnemyController::SearchMoveTargetUpdate(EnemyController* enemy)
{

}


/**
 * @brief 移動先決定ステート：終了
 */
void EnemyController::SearchMoveTargetExit(EnemyController* enemy)
{

}


/**
 * @brief 移動先決定ステート：遷移判定
 * @details 目的地が決定したため、即座に「移動(Move)」ステートへ遷移します。
 */
bool EnemyController::SearchMoveTargetCheck(EnemyController* enemy)
{
	// 目的地が決まったので、移動ステートへ遷移
	enemy->m_requestState = enAIType_Move;
	return true;
}