/**
 * Collisionが当たったかの判定処理をする
 */
#pragma once


/** コリジョンヒット監視クラス */
class CollisionHitManager
{


private:
    CollisionHitManager() {}
    ~CollisionHitManager() {}


public:
    void Update();




	/**
	 * シングルトン用
	 }*/
private:
	static CollisionHitManager* m_instance;


public:
    static CollisionHitManager* GetInstance()
    {
        return m_instance;
    }
    static void CreateInstance()
    {
        if (!m_instance)
            m_instance = new CollisionHitManager();
    }
    static void DeleteInstance()
    {
        if (m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }
};




class CollisionHitManagerObject : public IGameObject
{
public:
    CollisionHitManagerObject() {}
    ~CollisionHitManagerObject()
    {
        CollisionHitManager::DeleteInstance();
    }


    bool Start() override
    {
        CollisionHitManager::CreateInstance();
        return true;
    }
    void Update() override
    {
        CollisionHitManager::GetInstance()->Update();
    }
    void Render(RenderContext& rc) override {}
};
