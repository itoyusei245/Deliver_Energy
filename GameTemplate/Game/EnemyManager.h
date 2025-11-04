#pragma once
class Enemy;
class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();

private:
	static EnemyManager* m_instance;

private:
	/**可変長配列(Vector)を使ってEnemyを保存する*/
	std::vector<Enemy*> m_enemyList;

public:
	void Update();
	void Setup();
	void CreateEnemy(Vector3& pos);
	void RemoveEnemy();

public:
	/**EnemyManagerクラスのインスタンスを作成(NewGO)*/
	static void CreateInstance()
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

