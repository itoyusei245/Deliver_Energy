#include "stdafx.h"
#include "AthleticStage.h"
#include "AthleticState.h" // インクルード

AthleticStage::AthleticStage()
{
}

AthleticStage::~AthleticStage()
{
	// ステートのメモリ解放
	for (int i = 0; i < EnAthleticStateType_Max; i++) {
		if (m_stateList[i]) {
			delete m_stateList[i];
			m_stateList[i] = nullptr;
		}
	}
}

bool AthleticStage::Start()
{
	// 初期位置を保存
	m_defaultPosition = m_position;

	// ステートの生成
	m_stateList[enAthleticStateType_Idle] = new AthleticIdleState(this);
	m_stateList[enAthleticStateType_Move] = new AthleticMoveState(this);

	// 初期モデルロード
	m_modelRender.Init(m_modelPath.c_str());
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// ワールド行列を取得
	Matrix mat = m_modelRender.GetModel().GetWorldMatrix();

	mat.mat._41 = 0.0f;
	mat.mat._42 = 0.0f;
	mat.mat._43 = 0.0f;

	// 座標成分を0にした行列を使って物理オブジェクトを作成
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), mat);

	return true;
}
void AthleticStage::Update()
{
	// ステートの更新処理
	int requestState = EnAthleticStateType_Max;
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnAthleticStateType>(requestState);
		m_stateList[m_currentState]->Enter();
	}
	m_stateList[m_currentState]->Update();

	// 位置更新の反映
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// 物理の位置も更新
	m_physicsStaticObject.SetPosition(m_position);
}