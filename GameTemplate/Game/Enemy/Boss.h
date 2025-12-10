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
class Boss : public EnemyBase
{
	// NOTE: Boss用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	friend class BossIdleState;
	friend class BossMoveState;
	friend class BossCreateFamiliarState;


public:
	Boss();
	virtual ~Boss();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc);


public:
	BossStatus* GetStatus() { return dynamic_cast<BossStatus*>(m_status); }

	void SetRequestCreateFamiliar(const bool flg) { m_isRequestCreateFamiliar = flg; }
	bool IsRequestCreateFamiliar() const { return m_isRequestCreateFamiliar; }

	void SetCreatedFamiliar(const bool flg) { m_isCreatedFamiliar = flg; }
	bool IsCreatedFamiliar() const { return m_isCreatedFamiliar; }



private:
	EnBossStateType m_currentState = enBossStateType_Idle;
	// NOTE:ボスの状態の箱を作る
	// 配列の中をEnBossStateType_MaxにすることでEnBossStateTypeの中に状態を追加するだけで箱の数を追加できる
	// ステートごとに作ったクラスの情報を入れるための箱
	IBossState* m_stateList[EnBossStateType_Max];

	/** 眷属の生成をリクエスト */
	bool m_isRequestCreateFamiliar = false;
	/** 眷属生成済みか */
	bool m_isCreatedFamiliar = false;
};