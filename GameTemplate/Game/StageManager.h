#pragma once
class TriggerBox;
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
    TriggerBox* m_triggerBox = nullptr;
    CollisionObject* m_spawnCollisionObject = nullptr; 
    CollisionObject* m_stageCollisionObject = nullptr;

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

};
