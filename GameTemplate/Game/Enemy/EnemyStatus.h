/**
 * @file EnemyStatus.h
 * @brief 敵キャラクターのステータス管理クラス群定義
 */
#pragma once

 /**
  * @class IEnemyStatus
  * @brief 敵キャラクターのステータス基底インターフェース
  * @note 全ての敵のパラメータ（HP, 速度など）はこのクラスを継承して定義します。
  */
class IEnemyStatus
{
public:
	IEnemyStatus() {}
	virtual ~IEnemyStatus() {}

	/**
	 * @brief 初期パラメーターの設定
	 * @note 純粋仮想関数のため、継承先で必ず具体的な数値を設定してください。
	 */
	virtual void Setup() = 0;

public:
	/**
	 * @brief 最大HPを取得する
	 */
	int GetMaxHP() { return m_maxHP; }

	/**
	 * @brief 現在のHPを取得する
	 */
	int GetCurrentHP()const { return m_currentHP; }

	/**
	 * @brief 死亡判定
	 * @return bool HPが0以下ならtrue（死亡）
	 */
	bool IsDead()const { return m_currentHP <= 0; }

	/**
	 * @brief ダメージを与える
	 * @param[in] damgeValue ダメージ量
	 * @details HPを減算し、0未満にならないよう補正します。
	 */
	void Damage(int damageValue)
	{
		m_currentHP -= damageValue;
		if (m_currentHP < 0) {
			m_currentHP = 0;
		}
	}

	/**
	 * @brief 移動速度を取得する
	 */
	float GetMoveSpeed() const { return m_moveSpeed; }


protected:
	int m_maxHP = 1;        //!< 最大HP
	int m_currentHP = 1;    //!< 現在のHP
	float m_moveSpeed = 0.0f; //!< 移動速度
};


/**
 * @class BossStatus
 * @brief ボス敵専用のステータス
 */
class BossStatus : public IEnemyStatus
{
public:
	BossStatus() {}
	~BossStatus() {}

	/**
	 * @brief パラメータ初期化
	 * @details 3回の眷属召喚フェーズに対応するため、HPを3に設定します。
	 * （眷属セットを1回全滅させるごとに1ダメージ入る仕様のため）
	 */
	void Setup() override;

	/**
	 * @brief 眷属生成回数を加算する
	 * @details 戦闘フェーズを進めるために使用します。
	 */
	void AddCreatedFamiliarNum() { m_createdFamiliarNum++; }

	/**
	 * @brief 現在の眷属生成回数を取得する
	 * @return int 生成回数（0〜）
	 */
	int GetCreatedFamiliarNum() const { return m_createdFamiliarNum; }

protected:
	/** 眷属生成済み回数（フェーズ管理用） */
	int m_createdFamiliarNum = 0;
};


/**
 * @class FamiliarStatus
 * @brief ボスの眷属（中ボス/シールド役）のステータス
 */
class FamiliarStatus : public IEnemyStatus
{
public:
	FamiliarStatus() {}
	~FamiliarStatus() {}

	/**
	 * @brief パラメータ初期化
	 * @details HPは1。これを倒すことでボス本体にダメージが入ります。
	 */
	void Setup() override;
};


/**
 * @class NoobEnemyStatus
 * @brief 雑魚敵（巡回役）のステータス
 */
class NoobEnemyStatus : public IEnemyStatus
{
public:
	NoobEnemyStatus() {}
	~NoobEnemyStatus() {}

	/**
	 * @brief パラメータ初期化
	 */
	void Setup() override;
};