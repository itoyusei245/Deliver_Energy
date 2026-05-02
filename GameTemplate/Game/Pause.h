/**
 * @file Pause.h
 * @brief ポーズ画面クラス定義
 */
#pragma once
#include "UI/PauseUI.h"

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();

    void Update();
    void Render(RenderContext& rc);

private:
    PauseUI* m_ui = nullptr; 

    bool m_isActive = false;
    int m_selectBar = 0;
};