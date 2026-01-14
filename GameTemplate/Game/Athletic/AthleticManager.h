#pragma once
#include "AthleticStage.h"

class AthleticManager
{
private:
    AthleticManager();
    ~AthleticManager();


public:
    /**ƒVƒ“ƒOƒ‹ƒgƒ“*/
    static AthleticManager* GetInstance()
    {
        return m_instance;
    }
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new AthleticManager();
    }
    static void DeleteInstance()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    void Setup();

    void CreateStageTrigger(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
    void DeleteStageTrigger();
    CollisionObject* GetStageTrigger()const { return m_stageTrigger; }
    void SpawnAthleticStage();
   
private:
    static AthleticManager* m_instance;

    AthleticStage* m_currentStage = nullptr;
    CollisionObject* m_stageTrigger = nullptr;
};
