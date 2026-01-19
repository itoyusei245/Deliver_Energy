/**
 * @file AthleticStage.cpp
 * @brief アスレチックステージの実装
 */
#include "stdafx.h"
#include "AthleticStage.h"
#include "AthleticState.h" // インクルード

AthleticStage::AthleticStage()
{
}

AthleticStage::~AthleticStage()
{
	// ステートのメモリ解放
	// 配列に確保された各ステートインスタンスを削除する
	for (int i = 0; i < EnAthleticStateType_Max; i++) {
		if (m_stateList[i]) {
			delete m_stateList[i];
			m_stateList[i] = nullptr;
		}
	}
}

/**
 * @brief 開始処理
 * @details ステートの初期化と、モデル・物理オブジェクトの生成を行います。
 */
bool AthleticStage::Start()
{
	// 初期位置を保存（移動範囲計算の基準点となる）
	m_defaultPosition = m_position;

	// ステートの生成
	m_stateList[enAthleticStateType_Idle] = new AthleticIdleState(this);
	m_stateList[enAthleticStateType_Move] = new AthleticMoveState(this);

	// 初期モデルロード
	m_modelRender.Init(m_modelPath.c_str());
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// ワールド行列を取得して物理メッシュを作成する
	Matrix mat = m_modelRender.GetModel().GetWorldMatrix();

	// 平行移動成分を削除（物理オブジェクト作成時は原点基準で形状を作り、後でSetPositionで動かすため）
	mat.mat._41 = 0.0f;
	mat.mat._42 = 0.0f;
	mat.mat._43 = 0.0f;

	// 座標成分を0にした行列を使って物理オブジェクトを作成
	// これにより回転やスケールは反映されるが、位置オフセットはリセットされた形状が作られる
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), mat);

	return true;
}

/**
 * @brief 更新処理
 * @details ステートマシンの駆動と、オブジェクトの位置更新を行います。
 */
void AthleticStage::Update()
{
	// --- ステートの更新処理 ---
	int requestState = EnAthleticStateType_Max;

	// 現在のステートを実行し、遷移リクエストがあるか確認
	if (m_stateList[m_currentState]->RequestState(requestState)) {
		// 遷移がある場合：現在のステートを終了 -> 新しいIDをセット -> 新しいステートを開始
		m_stateList[m_currentState]->Exit();
		m_currentState = static_cast<EnAthleticStateType>(requestState);
		m_stateList[m_currentState]->Enter();
	}

	// 現在のステートの更新を実行
	m_stateList[m_currentState]->Update();

	// --- 位置情報の反映 ---
	// ステート内で変更された m_position を描画モデルに反映
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// 物理判定（StaticObject）の位置も更新して同期させる
	m_physicsStaticObject.SetPosition(m_position);
}