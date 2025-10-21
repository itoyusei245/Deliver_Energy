#pragma once
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

public:
	static StageManager* CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new StageManager();
		}
	}
	static int DeleteInstance()
	{
		if (m_instance!=nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
	static StageManager* GetInstance()
	{
		return m_instance;
	}
};

