/**
 * EnemyStatus.h
 * 敵キャラクターのステータス
 */
#pragma once


/**
 * 敵キャラクターのステータス基底クラス
 */
class IEnemyStatus
{
public:
	IEnemyStatus() {}
	virtual ~IEnemyStatus() {}
	
	/**
	 * 初期パラメーターの設定
	 * NOTE: 必ず継承先で実装してください
	 */
	virtual void Setup() = 0;


public:
	int GetMaxHP() { return m_maxHP; }


	//現在のHPを取得する関数
	int GetCurrentHP()const { return m_currentHP; }


	//死亡判定
	bool IsDead()const { return m_currentHP <= 0; }


	void Damage(int damgeValue)
	{
		m_currentHP -= damgeValue;
		if (m_currentHP < 0) {
			m_currentHP = 0;
		}
	}


	float GetMoveSpeed() const { return m_moveSpeed; }



protected:
	int m_maxHP = 1;
	int m_currentHP = 1;
	float m_moveSpeed = 0.0f;
};




/** ボス敵のステータス */
class BossStatus : public IEnemyStatus
{
public:
	BossStatus() {}
	~BossStatus() {}

	void Setup() override
	{
		// 初期パラメーターの設定
		// 3体の眷属に対応するため、HPを3にする（1体倒すごとに1ダメージ）
		m_maxHP = 3;
		m_currentHP = m_maxHP;
		m_moveSpeed = 100.0f;
	}


	void AddCreatedFamiliarNum() { m_createdFamiliarNum++; }
	int GetCreatedFamiliarNum() const { return m_createdFamiliarNum;}


protected:
	int m_createdFamiliarNum = 0;
};




/** ボスの分身体のステータス */
class FamiliarStatus : public IEnemyStatus
{
public:
	FamiliarStatus() {}
	~FamiliarStatus() {}

	void Setup() override
	{
		// 初期パラメーターの設定
		m_maxHP = 1;
		m_currentHP = m_maxHP;
		m_moveSpeed = 100.0f;
	}



protected:

};




/** 雑魚敵の分身体のステータス */
class NoobEnemyStatus : public IEnemyStatus
{
public:
	NoobEnemyStatus() {}
	~NoobEnemyStatus() {}

	void Setup() override
	{
		// 初期パラメーターの設定
		m_maxHP = 1;
		m_currentHP = m_maxHP;
		m_moveSpeed = 100.0f;
	}



protected:

};