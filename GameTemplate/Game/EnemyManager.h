#pragma once
class Boss;
class Familiar;

class EnemyManager
{
private:
    EnemyManager();
    ~EnemyManager();

public:
    /**シングルトン*/
    static EnemyManager* GetInstance()
    {
        return m_instance;
    }
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new EnemyManager();
    }
    static void DeleteInstance()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    void Update();                 /**毎フレーム更新*/
    void Setup();                  /**初期化（現状は空)*/

    /**BossEnemy を生成してリストに追加*/
    void CreateEnemy(const Vector3& pos);

private:
    static EnemyManager* m_instance;

    /**BossEnemy だけ管理*/
    Boss* m_boss = nullptr;
    Familiar* m_familia = nullptr;
};
