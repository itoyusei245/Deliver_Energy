/**
 * @file AthleticManager.h
 * @brief アスレチックステージ全体の管理クラス定義
 */
#pragma once
#include "AthleticStage.h"

 /**
  * @class AthleticManager
  * @brief アスレチックステージの進行管理クラス
  * @note シングルトンパターンで実装されており、ステージへのトリガー生成やステージ本体のスポーンを管理します。
  */
class AthleticManager
{
private:
    /**
     * @brief コンストラクタ
     * @note シングルトンのためprivate
     */
    AthleticManager();

    /**
     * @brief デストラクタ
     */
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
     * @note ゲーム開始時などに一度だけ呼び出してください。
     */
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new AthleticManager();
    }

    /**
     * @brief インスタンスを破棄する
     * @note ゲーム終了時などに呼び出し、メモリを解放してください。
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
     * @details トリガー検知用オブジェクト(AthleticTrigger)の生成などを行います。
     */
    void Setup();

    /**
     * @brief ステージ開始用のトリガー（当たり判定）を作成する
     * @param[in] pos 配置座標
     * @param[in] rot 回転情報
     * @param[in] scale サイズ（スケール）
     */
    void CreateStageTrigger(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

    /**
     * @brief ステージ開始用のトリガーを削除する
     */
    void DeleteStageTrigger();

    /**
     * @brief 現在のステージトリガーを取得する
     * @return CollisionObject* トリガーオブジェクトへのポインタ
     */
    CollisionObject* GetStageTrigger()const { return m_stageTrigger; }

    /**
     * @brief アスレチックステージ本体を生成する
     * @details プレイヤーがトリガーに接触した際などに呼び出し、実際のステージギミックを展開します。
     */
    void SpawnAthleticStage();

private:
    /** シングルトンインスタンス */
    static AthleticManager* m_instance;

    /** 現在のアスレチックステージ本体 */
    AthleticStage* m_currentStage = nullptr;

    /** ステージ突入判定用のトリガーオブジェクト */
    CollisionObject* m_stageTrigger = nullptr;
};