#pragma once
#include "GameStateBase.h"


class Player;
class GameCamera;
class BackGround;
class EnemyManager;
class StageManager;
class Countdown;
class GameTimeUI;
class CoinUI;
class Pause;


class InGameState:public GameStateBase
{
public:
	InGameState();
	~InGameState();


	void OnUpdate();
	void OnEnter();
	void OnExit();
	bool ShouldChangeState();


	void Render(RenderContext& rc);


	void DeleteGameObjects();


	static bool IsGamePlay;
	static bool IsPaused;
	static int CoinCount;


private:
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
	BackGround* m_backGround = nullptr;
	Countdown* m_countdown = nullptr;
	GameTimeUI* m_gameTimeUI = nullptr;
	CoinUI* m_coinUI = nullptr;
	Pause* m_pause = nullptr;
};

