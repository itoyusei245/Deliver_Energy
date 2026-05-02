/**
 * @file Setting.h
 * @brief 設定画面（ロジック）クラス定義
 */
#pragma once
#include "UI/SettingUI.h"

class Setting : public IGameObject
{
public:
	Setting();
	~Setting();

	void Update() override;
	void Render(RenderContext& rc) override;

private:
	SettingUI* m_ui = nullptr;

	int m_currentBar = 0; 

	void UpdatePicUp();
	void UpdateVolumeControl();
};