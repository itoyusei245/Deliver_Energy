/**
 * @file Familiar.cpp
 * @brief 眷属クラスの実装
 */
#include "stdafx.h"
#include "Familiar.h"

Familiar::Familiar()
{
	// ステータスの生成とセットアップ
	m_status = new FamiliarStatus();
	m_status->Setup();
}

Familiar::~Familiar()
{
	// ステータスの解放
	delete m_status;
	m_status = nullptr;

	// ステートリストの解放
	for (auto* state : m_stateList) {
		delete state;
	}
}

/**
 * @brief 開始処理
 * @details
 * 1. スケール設定
 * 2. ステート(Idle, Move)の生成
 * 3. タイプ(A/B)に応じた3Dモデルの読み込み
 * 4. 物理オブジェクト(PhysicsStaticObject)と当たり判定(Sphere)の生成
 */
bool Familiar::Start()
{
	SetScale(Vector3(1.7f, 1.7f, 1.7f));

	// ステート生成
	m_stateList[enFamiliarStateType_Idle] = new FamiliarIdleState(this);
	m_stateList[enFamiliarStateType_Move] = new FamiliarMoveState(this);

	// タイプによってモデルを切り替える
	switch (m_familiaType)
	{
	case enFamiliarType_A:
		m_modelRender.Init("Assets/animData/bossEnemy_TypeA.tkm");
		break;
	case enFamiliarType_B:
		m_modelRender.Init("Assets/animData/m_boss_Enemy_TypeB.tkm");
		break;
	default:
		break;
	}

	// モデルの初期配置更新
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();

	// モデル形状から物理メッシュを作成
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());


	// (コメントアウトされているコアモデル処理)
	//m_coreModel.Init("Assets/animData/familiarCore.tkm");
	//m_coreModel.SetTRS(Vector3::Zero, m_rotation, m_scale);
	//m_coreModel.Update();
	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// 当たり判定用球体の作成（半径110.0f）
	m_collisionObject.CreateSphere(m_position, m_rotation, 110.0f);

	return true;
}

/**
 * @brief 更新処理
 * @details
 * - ステートマシンの遷移制御と実行
 * - オブジェクトの位置更新（物理、モデル、コリジョンを現在の座標に同期）
 */
void Familiar::Update()
{
	// --- ステートマシンの更新 ---
	int requestState = EnBossStateType_Max;

	// 遷移リクエストがあれば切り替え
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnFamiliarStateType>(requestState);
		m_stateList[m_currentState]->Eneter(); // Typo: Enter
	}
	// 現在のステートの更新
	m_stateList[m_currentState]->Update();

	// --- 座標同期 ---
	// 物理オブジェクトの位置更新
	m_physicsStaticObject.SetPosition(m_position);

	// モデルの行列更新
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	// コリジョンの位置更新
	m_collisionObject.SetPosition(m_position);
	m_collisionObject.Update();
}

/**
 * @brief 描画処理
 */
void Familiar::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

/**
 * @brief タイプのセットアップ
 * @details EnemyManager等から生成直後に呼び出されます。
 */
void Familiar::Setup(const enFamiliarType type)
{
	m_familiaType = type;
}