/**
 * EnemyBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once
#include "EnemyBase.h"
#include "EnemyStatePattern.h"


/**
 * 敵キャラクターの基底クラス
 * NOTE: 敵キャラクターを作るときは継承してください
 */
class NoobEnemy : public EnemyBase
{
	// NOTE:  NoobEnemy用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	friend class  NoobEnemyIdleState;


public:
	NoobEnemy();
	virtual ~NoobEnemy();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc);


public:
	NoobEnemyStatus* GetStatus() { return dynamic_cast<NoobEnemyStatus*>(m_status); }


private:
	EnNoobEnemyStateType m_currentState = enNoobEnemyStateType_Idle;
	// NOTE:雑魚敵の状態の箱を作る
	// 配列の中をEnNoobEnemyStateType_MaxにすることでEnNoobEnemyStateTypeの中に状態を追加するだけで箱の数を追加できる
	// ステートごとに作ったクラスの情報を入れるための箱
	INoobEnemyState* m_stateList[EnNoobEnemyStateType_Max];
};