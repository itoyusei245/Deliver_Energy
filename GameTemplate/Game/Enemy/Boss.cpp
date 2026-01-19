/**
 * @file Boss.cpp
 * @brief ボスキャラクターの実装
 */
#include "stdafx.h"
#include "Boss.h"

Boss::Boss()
{
	// ステータスのメモリ確保
	m_status = new BossStatus();
	m_status->Setup();
}

Boss::~Boss()
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
 * 1. スケールの設定
 * 2. 各ステート(Idle, Move, CreateFamiliar)の生成
 * 3. 3Dモデル(.tkm)のロードと初期配置
 * 4. モデル形状に合わせた物理メッシュ(PhysicsStaticObject)の作成
 * 5. 当たり判定用スフィア(CollisionObject)の作成
 */
bool Boss::Start()
{
	SetScale(Vector3(65.0f, 65.0f, 65.0f));

	// ステートの生成
	m_stateList[enBossStateType_Idle] = new BossIdleState(this);
	m_stateList[enBossStateType_Move] = new BossMoveState(this);
	m_stateList[enBossStateType_CreateFamiliar] = new BossCreateFamiliarState(this);

	// モデル初期化
	m_modelRender.Init("Assets/animData/main_bossEnemy.tkm");
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();

	// 物理オブジェクト作成（モデルの形状を利用）
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// 当たり判定用球体の作成
	m_collisionObject.CreateSphere(m_position, m_rotation, 10.0f);
	m_collisionObject.Update();

	return true;
}

/**
 * @brief 更新処理
 * @details
 * ステートマシンの遷移制御と更新を行います。
 * また、ボスの現在座標を物理オブジェクトとモデルに反映させます。
 */
void Boss::Update()
{
	// --- ステートマシンの更新 ---
	int requestState = EnBossStateType_Max;

	// 現在のステートから遷移リクエストがあるか確認
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		// 現在のステートを終了
		m_stateList[m_currentState]->Exit();
		// 新しいステートIDに切り替え
		m_currentState = static_cast<EnBossStateType>(requestState);
		// 新しいステートを開始
		m_stateList[m_currentState]->Eneter(); // ※ここはおそらくEnterのタイポですが、元のコードに合わせています
	}
	// 現在のステートの更新を実行
	m_stateList[m_currentState]->Update();

	// --- 座標同期 ---
	// 物理オブジェクトの位置更新
	m_physicsStaticObject.SetPosition(m_position);

	// モデルの行列更新
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

/**
 * @brief 描画処理
 */
void Boss::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}