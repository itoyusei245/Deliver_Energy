/**
 * @file NoobEnemy.cpp
 * @brief 雑魚敵の実装
 */
#include "stdafx.h"
#include "NoobEnemy.h"


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
	SetScale(Vector3(1.7f, 1.7f, 1.7f));

	// ステートの生成
	m_stateList[enNoobEnemyStateType_Idle] = new NoobEnemyIdleState(this);
	m_stateList[enNoobEnemyStateType_Move] = new NoobEnemyMoveState(this);

	// モデル初期化
	m_modelRender.Init("Assets/animData/noobEnemy.tkm");
	m_modelRender.SetTRS(Vector3::Zero, m_rotation, m_scale);
	m_modelRender.Update();

	// 物理オブジェクト作成（モデルの形状を利用）
	// NOTE: MoveState内で座標を直接更新するため、StaticObjectとして作成し、Kinematicな挙動をさせます。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// 当たり判定用球体（半径10.0f）
	m_collisionObject.CreateSphere(m_position, m_rotation, 10.0f);
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
	// --- ステート更新 ---
	int requestState = EnNoobEnemyStateType_Max;

	// 遷移リクエストがあれば切り替え
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnNoobEnemyStateType>(requestState);
		m_stateList[m_currentState]->Eneter(); // Typo: Enter
	}
	// 現在のステート更新
	m_stateList[m_currentState]->Update();

	// --- 座標同期 ---
	// 物理座標の更新
	m_physicsStaticObject.SetPosition(m_position);

	//死亡チェック
	if (m_status !=nullptr&&m_status->IsDead())
	{
		//共通の死亡処理を呼ぶ
		OnDead();


		//自身を消去
		DeleteGO(this);
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