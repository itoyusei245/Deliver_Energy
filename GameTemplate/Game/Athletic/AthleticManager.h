/**
 * @file AthleticManager.h
 * @brief アスレチックステージ全体の管理クラス定義
 */
#pragma once
#include "AthleticStage.h"

 /**
  * @class AthleticManager
  * @brief アスレチックステージの進行管理クラス
  */
class AthleticManager
{
private:
    AthleticManager();
    ~AthleticManager();


public:
    /**
     * @brief インスタンスを取得する
     * @return AthleticManager* インスタンスへのポインタ
     */
    static AthleticManager* GetInstance()
    {
        return m_instance;
    }

    /**
     * @brief インスタンスを生成する
     * @note ゲーム開始時などに一度だけ呼び出す。
     */
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new AthleticManager();
    }

    /**
     * @brief インスタンスを破棄する
     * @note ゲーム終了時などに呼び出し、メモリを解放する。
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
     * @brief 初期セットアップを行う
     * @details トリガー検知用オブジェクト(AthleticTrigger)の生成などを行う。
     */
    void Setup();

    void CreateStageTrigger(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

    void DeleteStageTrigger();

    CollisionObject* GetStageTrigger()const { return m_stageTrigger; }

    /**
     * @brief アスレチックステージ本体を生成する
     * @details プレイヤーがトリガーに接触した際などに呼び出し、実際のステージギミックを展開する。
     */
    void SpawnAthleticStage();

private:
    static AthleticManager* m_instance;

    /** 現在のアスレチックステージ本体 */
    AthleticStage* m_currentStage = nullptr;

    /** ステージ突入判定用のトリガーオブジェクト */
    CollisionObject* m_stageTrigger = nullptr;
};