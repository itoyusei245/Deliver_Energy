/**
 * @file Title.h
 * @brief タイトル画面（ロジック）クラス定義
 */
#pragma once
#include "UI/TitleUI.h"

class Title : public IGameObject
{
public:
    Title();
    ~Title();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    /** タイトル画面かどうか */
    static bool IsTitle;

    /** 設定画面を開いているかどうか */
    static bool IsSetting;

private:
    TitleUI* m_ui = nullptr;
    int m_currentBar = 0;

    void UpdatePicUp();
};