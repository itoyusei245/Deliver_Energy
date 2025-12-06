#pragma once
#include <memory>


class GameStateBase : public IGameObject
{
public:
	GameStateBase() = default;
	virtual ~GameStateBase() = default;
	bool Start()override;

	void Render(RenderContext& rc)override;
	virtual bool ShouldChangeState() = 0;
	virtual GameStateBase* ChangeState()final;
	virtual void Update() final;
	virtual void OnUpdate() = 0;
	virtual void Enter() final;
	virtual void OnEnter() = 0;
	virtual void Exit() final;
	virtual void OnExit() = 0;
	virtual void SetNextName(const char* nextStateName)final;
	virtual  std::unique_ptr<GameStateBase> NextState() final;


	void SetActive(bool flag);
	std::unique_ptr<GameStateBase> m_nextState = nullptr;
	const char* m_nextStateName;
};