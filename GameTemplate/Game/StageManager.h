#pragma once
#include <vector>
#include <string>

// 1個の生成物についての情報
// ファイルパスだけでなく、動きのパラメータも持てるようにする
struct AthleticSpawnInfo {
    std::string filePath;
    float moveSpeed = 0.0f; // 移動速度 (0なら動かない)
    float moveRange = 0.0f; // 上下移動の振れ幅

    // コンストラクタで初期化しやすくしておく
    AthleticSpawnInfo(const char* path, float speed = 0.0f, float range = 0.0f)
        : filePath(path), moveSpeed(speed), moveRange(range) {
    }
};

/**トリガーと生成リストをセットで管理する構造体*/
struct AthleticTriggerInfo {
    CollisionObject* trigger;
    std::vector<AthleticSpawnInfo> spawnInfos; 
};

class StageManager
{
private:
    StageManager();
    ~StageManager();

public:
    void Update();
    void Setup();

private:
    static StageManager* m_instance;
    CollisionObject* m_spawnCollisionObject = nullptr;
    CollisionObject* m_stageCollisionObject = nullptr;

    std::vector<AthleticTriggerInfo> m_athleticTriggers;

public:
    static void CreateInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new StageManager();
        }
    }
    static void DeleteInstance()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    static StageManager* GetInstance()
    {
        return m_instance;
    }

    void CreateEnemyBox(Vector3& pos, Quaternion& rot, Vector3& size);
    void CreateStageBox(Vector3& pos, Quaternion& rot, Vector3& size);
    void CreateAthleticBox(Vector3& pos, Quaternion& rot, Vector3& size, const std::vector<AthleticSpawnInfo>& spawnInfos);
};