/**
 * @file EnemyManager.h
 * @brief 敵キャラクター全体の管理クラス定義
 */
#pragma once

 // 前方宣言
class Boss;
class Familiar;
class BossStatusUI;

/**
 * @class EnemyManager
 * @brief 敵の生成、削除、およびボス戦のルール進行を管理するクラス
 * @note シングルトンパターンで実装されています。
 * ボスと眷属（Familiar）の連携や、眷属死亡時のボスへのダメージ処理など、
 * 戦闘のフェーズ進行もこのクラスが担当します。
 */
class EnemyManager
{
private:
    EnemyManager();
    ~EnemyManager();

public:
    /**
     * @brief インスタンスを取得する
     * @return EnemyManager* インスタンスへのポインタ
     */
    static EnemyManager* GetInstance()
    {
        return m_instance;
    }

    /**
     * @brief インスタンスを生成する
     */
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new EnemyManager();
    }

    /**
     * @brief インスタンスを破棄する
     */
    static void DeleteInstance()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }


    /**
     * @brief 現在のボスインスタンスを取得する
     * @return Boss* ボスへのポインタ
     */
    Boss* GetBoss()const { return m_boss; }


    /**
     * @brief 毎フレーム更新処理
     * @details 眷属の生成リクエストの処理や、眷属死亡時のボスへのダメージ反映を行います。
     */
    void Update();

    /**
     * @brief 初期化処理
     * @note 現状は空実装です。
     */
    void Setup();

    /**
     * @brief ボスとUIを生成する
     * @param[in] pos ボスの初期座標
     */
    void CreateEnemy(const Vector3& pos);


public:
    bool IsBossClearEventRequested()const { return m_isBossClearEventRequested; }
    void ClearBossClearEventRequest() { m_isBossClearEventRequested = false; }


private:
    static EnemyManager* m_instance;

    /** 現在管理しているボス */
    Boss* m_boss = nullptr;

    /** * 現在管理している眷属（中ボス/シールド役）
     * @note この眷属を倒すことでボスにダメージが入る仕組みです。
     */
    Familiar* m_familia = nullptr;

    /** 眷属のタイプ切り替え用フラグ（Aタイプ/Bタイプ交互生成用） */
    bool m_isChangeType = true;

    /** ボスのHPバーなどのUI */
    BossStatusUI* m_bossUI = nullptr;

    // 眷属の死亡遅延用
    bool m_isFamiliarDying = false;
    int m_familiarDeathTimer = 0;

    bool m_isBossClearEventRequested = false;

    // ボスの死亡遅延用
    bool m_isBossDying = false;
    int m_bossDeathTimer = 0;
};