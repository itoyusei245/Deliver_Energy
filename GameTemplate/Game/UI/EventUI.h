#pragma once
#include "UIBase.h"

class EventUI : public IGameObject
{
public:
    EventUI();
    ~EventUI();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    UICanvas* m_canvas = nullptr;
    UIIcon* m_icon = nullptr;
};