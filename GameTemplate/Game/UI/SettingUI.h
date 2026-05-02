/**
 * @file SettingUI.h
 * @brief 設定画面の描画のみを担当するUIクラス
 */
#pragma once
#include "UIBase.h"

class SettingUI
{
public:
	SettingUI();
	~SettingUI();

	void Update();
	void Render(RenderContext& rc);

	// ロジック側から状態を更新するための関数
	void SetCurrentBar(int index);
	void SetVolumes(float master, float bgm, float se);

private:
	UICanvas* m_canvas = nullptr;

	UIIcon* m_settingBg = nullptr;       // 背景画像
	UIIcon* m_picUpIcon = nullptr;       // 現在選ばれている項目のハイライト枠

	// 枠線
	UIIcon* m_barMasterFrame = nullptr;
	UIIcon* m_barBgmFrame = nullptr;
	UIIcon* m_barSeFrame = nullptr;

	// ゲージ中身
	UIGauge* m_barMaster = nullptr;
	UIGauge* m_barBgm = nullptr;
	UIGauge* m_barSe = nullptr;

	int m_currentBarIndex = -1;
};