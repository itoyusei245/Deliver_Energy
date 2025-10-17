#pragma once
class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();

public:
	void Update();
	void Setup();

private:
	
	static EnemyManager* m_instance;

public:
	/**EnemyManagerクラスのインスタンスを作成(NewGO)*/
	static EnemyManager* CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new EnemyManager();
		}
	}
	/**EnemyManagerクラスのインスタンスを削除(DeleteGO)*/
	static void DeleteInstance()
	{
		if (m_instance!=nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
	/**EnemyManagerクラスのインスタンスを取得(FindGO)*/
	static EnemyManager*GetInstance()
	{
		return m_instance;
	}

	
};

