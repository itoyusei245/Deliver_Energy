/**
 * @file StageManager.h
 * @brief ステージ上のオブジェクト生成管理クラス定義
 */
#pragma once
#include <vector>
#include <string>

 /**
  * @struct AthleticSpawnInfo
  * @brief アスレチック（ギミック）生成用の定義情報
  */
struct AthleticSpawnInfo {
    std::string filePath;   /**< モデルファイルパス */
    float moveSpeed = 0.0f; /**< 移動速度 (0なら静止) */
    float moveRange = 0.0f; /**< 移動範囲 */

    AthleticSpawnInfo(const char* path, float speed = 0.0f, float range = 0.0f)
        : filePath(path), moveSpeed(speed), moveRange(range) {
    }
};

/**
 * @struct AthleticTriggerInfo
 * @brief トリガーと、それに紐づく生成リストのペア
 */
struct AthleticTriggerInfo {
    CollisionObject* trigger; /**< 接触判定用トリガー */
    std::vector<AthleticSpawnInfo> spawnInfos; /**< トリガー接触時に生成されるオブジェクト群 */
};

/**
 * @class StageManager
 * @brief ステージギミックや敵の生成を管理するクラス
 * @details シングルトンパターン。トリガーとプレイヤーの接触を監視し、
 * 条件を満たした際に敵やアスレチック（足場など）を動的に生成します。
 */
class StageManager
{
private:
    StageManager();
    ~StageManager();

public:
    void Update();
    void Setup();

    // ボスのイベント発生リクエストがあるかどうか
    bool IsBossEventRequested() { return m_isBossEventRequested; }

    // リクエストを消化（オフに）する関数
    void ClearBossEventRequest() { m_isBossEventRequested = false; }

    /** @name 生成メソッド */
    /*@{*/
    void CreateEnemyBox(Vector3& pos, Quaternion& rot, Vector3& size);
    void CreateStageBox(Vector3& pos, Quaternion& rot, Vector3& size);
    /** * @brief アスレチック生成用トリガーの登録
     * @param spawnInfos 生成するオブジェクト情報のリスト
     */
    void CreateAthleticBox(Vector3& pos, Quaternion& rot, Vector3& size, const std::vector<AthleticSpawnInfo>& spawnInfos);
    /*@}*/

    // --- シングルトン管理 ---
    static void CreateInstance()
    {
        if (m_instance == nullptr) m_instance = new StageManager();
    }
    static void DeleteInstance()
    {
        if (m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    static StageManager* GetInstance()
    {
        return m_instance;
    }

private:
    static StageManager* m_instance;

    // フラグ変数
    bool m_isBossEventRequested = false;

    CollisionObject* m_spawnCollisionObject = nullptr;
    CollisionObject* m_stageCollisionObject = nullptr;

    /** 登録されたアスレチックトリガーのリスト */
    std::vector<AthleticTriggerInfo> m_athleticTriggers;
};