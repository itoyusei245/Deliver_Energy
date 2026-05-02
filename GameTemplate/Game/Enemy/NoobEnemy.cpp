/**
 * @file NoobEnemy.cpp
 * @brief 雑魚敵の実装
 */
#include "stdafx.h"
#include "NoobEnemy.h"
#include "EnemyManager.h"
#include "Boss.h"        

namespace 
{
	constexpr const char* MODEL_PATH	 = "Assets/animData/noobEnemy.tkm";
	constexpr const char* NAME_EFF_DEATH = "noobDeathEffect";

	constexpr float COLLISION_RADIUS = 10.0f;
	constexpr float DEATH_EFF_OFFSET_Y = 50.0f;

	const Vector3	NOOB_SCALE(1.7f, 1.7f, 1.7f);
	const Vector3	DEATH_EFF_SCALE(3.0f, 3.0f, 3.0f);
	constexpr int	WAIT_FRAME_DEATH   = 10;
}

NoobEnemy::NoobEnemy()
{
	// ステータスの生成
	m_status = new NoobEnemyStatus();
	m_status->Setup();
}


NoobEnemy::~NoobEnemy()
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
 * 1. ステート(Idle, Move)の生成
 * 2. 3Dモデル("Assets/animData/noobEnemy.tkm")のロード
 * 3. モデル形状に基づいた物理メッシュ(PhysicsStaticObject)の作成
 * 4. 当たり判定用球体(CollisionObject)の作成
 */
bool NoobEnemy::Start()
{
	SetScale(NOOB_SCALE);

	// ステートの生成
	m_stateList[enNoobEnemyStateType_Idle] = new NoobEnemyIdleState(this);
	m_stateList[enNoobEnemyStateType_Move] = new NoobEnemyMoveState(this);

	// モデル初期化
	m_modelRender.Init(MODEL_PATH);
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// 当たり判定用球体（半径10.0f）
	m_collisionObject.CreateSphere(m_position, m_rotation, COLLISION_RADIUS);
	m_collisionObject.Update();

	return true;
}


/**
 * @brief 更新処理
 * @details
 * - ステートマシンの更新を実行（ここでAIからセットされたベクトルに基づいて座標が変更される）
 * - 変更された座標を物理オブジェクトとモデルに反映（同期）
 */
void NoobEnemy::Update()
{
	if (EnemyManager::GetInstance() == nullptr) return;
	// ---------------------------------------------------
	// ボスの死亡を検知して、消滅する
	// ---------------------------------------------------
	Boss* boss = EnemyManager::GetInstance()->GetBoss();

	// ボスが存在し、かつボスのHPが0（死んでいる）なら
	if (boss != nullptr && boss->GetStatus()->IsDead()) {
		// 自分自身をパッと消去して、これ以降の処理を行わない
		DeleteGO(this);
		return;
	}

	// --- ステート更新 ---
	int requestState = EnNoobEnemyStateType_Max;

	// 遷移リクエストがあれば切り替え
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnNoobEnemyStateType>(requestState);
		m_stateList[m_currentState]->Enter(); // Typo: Enter
	}
	// 現在のステート更新
	m_stateList[m_currentState]->Update();

	// --- 座標同期 ---
	// 物理座標の更新
	m_physicsStaticObject.SetPosition(m_position);

	m_collisionObject.SetPosition(m_position);
	m_collisionObject.Update();

	//死亡チェック
	if (m_status !=nullptr&&m_status->IsDead())
	{
		if (!m_isDying) {
			m_isDying = true;
			OnDead();

			//エフェクト再生
			EffectEmitter* effect = NewGO<EffectEmitter>(0, NAME_EFF_DEATH);
			effect->Init(0);
			Vector3 effectPos = m_position;
			effectPos.y += DEATH_EFF_OFFSET_Y; // 雑魚敵の中心に合わせる
			effect->SetPosition(effectPos);
			effect->SetScale(DEATH_EFF_SCALE);
			effect->Play();
			effect->Update();
		}


		// タイマーを進める
		m_deathTimer++;

		// 指定フレーム待ってから完全に消す
		if (m_deathTimer >= WAIT_FRAME_DEATH) {
			DeleteGO(this);
		}

		// 死んでいる間は移動やAIの更新をしたくないので、ここで return する
		return;
	}

	// モデル座標の更新
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}


/**
 * @brief 描画処理
 */
void NoobEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}