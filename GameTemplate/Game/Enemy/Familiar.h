/**
 * EnemyBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once
#include "EnemyBase.h"
#include "EnemyStatePattern.h"

enum enFamiliarType
{
	enFamiliarType_A,
	enFamiliarType_B,
};


/**
 * 敵キャラクターの基底クラス
 * NOTE: 敵キャラクターを作るときは継承してください
 */
class Familiar : public EnemyBase
{
	// NOTE: Familiar用のステートパターンでアクセスしたいので例外的にfriendクラスにする
	friend class FamiliarIdleState;


public:
	Familiar();
	virtual ~Familiar();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc);


public:
	void Setup(const enFamiliarType type);


public:
	FamiliarStatus* GetStatus() { return dynamic_cast<FamiliarStatus*>(m_status); }


private:
	EnFamiliarStateType m_currentState = enFamiliarStateType_Idle;
	// NOTE:ボスの分身体の状態の箱を作る
	// 配列の中をEnFamiliarStateType_MaxにすることでEnFamiliarStateTypeの中に状態を追加するだけで箱の数を追加できる
	// ステートごとに作ったクラスの情報を入れるための箱
	IFamiliarState* m_stateList[EnFamiliarStateType_Max];


private:
	ModelRender m_coreModel;
	enFamiliarType m_familiaType;
};