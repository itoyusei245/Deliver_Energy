/**
 * @file GameStateBase.cpp
 * @brief ゲームステート基底クラスの実装
 */
#include "stdafx.h"
#include "GameStateBase.h"
#include <memory>

 /**
  * @brief 初期化処理
  * @details 生成直後は非アクティブ状態にしておきます（ChangeStateで呼ばれるまで待機）。
  */
bool GameStateBase::Start()
{
	Deactivate(); // IGameObjectの機能を呼び出し（Update等を停止）
	return true;
}

void GameStateBase::Render(RenderContext& rc)
{
}

/**
 * @brief ステートの切り替え実行
 * @details
 * 1. 自身を非アクティブ化(SetActive(false))
 * 2. 次のステート名を元にオブジェクトを検索(FindGO)
 * 3. 見つかった次のステートをアクティブ化(SetActive(true))
 * @return GameStateBase* 次のステートのポインタ
 */
GameStateBase* GameStateBase::ChangeState()
{
	SetActive(false);
	GameStateBase* nextState = FindGO<GameStateBase>(m_nextStateName);
	if (nextState) {
		nextState->SetActive(true);
	}
	return nextState;
}

void GameStateBase::SetNextName(const char* nextStateName)
{
	m_nextStateName = nextStateName;
}

/**
 * @brief 更新ループ
 * @details アクティブフラグ(m_isActive)がtrueの場合のみ、派生クラスのOnUpdateを実行します。
 */
void GameStateBase::Update()
{
	if (m_isActive) {
		OnUpdate();
	}
}

void GameStateBase::OnUpdate()
{
	// 派生クラスで実装されます
}

/**
 * @brief ステート突入時の処理
 * @details エンジン側のActivate()を呼び、自前のフラグ管理等のOnEnter()を実行します。
 */
void GameStateBase::Enter()
{
	Activate(); // IGameObjectのアクティブ化
	OnEnter();
}

/**
 * @brief ステート脱出時の処理
 * @details エンジン側のDeactivate()を呼び、後始末用のOnExit()を実行します。
 */
void GameStateBase::Exit()
{
	Deactivate(); // IGameObjectの非アクティブ化
	OnExit();
}


std::unique_ptr<GameStateBase> GameStateBase::NextState()
{

	if (!m_nextState) {
		return nullptr;
	}
	return std::move(m_nextState);
}

/**
 * @brief アクティブフラグの設定
 * @note このクラス独自のフラグ(m_isActive)を設定します。IGameObjectのActivate/Deactivateとは別管理のようです。
 */
void GameStateBase::SetActive(bool flag)
{
	m_isActive = flag;
}