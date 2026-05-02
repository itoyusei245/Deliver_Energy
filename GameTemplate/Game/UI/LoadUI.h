#pragma once
#include"UIBase.h"

class LoadUI : public IGameObject
{
public:
    LoadUI();
    ~LoadUI();
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
	UICanvas *m_canvas = nullptr;
	UIIcon *m_icon = nullptr;
};