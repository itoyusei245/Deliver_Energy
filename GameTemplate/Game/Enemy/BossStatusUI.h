/**
 * @file BossStatusUI.h
 * @brief ボスのアイコンとHPバーを表示するUIクラス定義
 */
#pragma once
#include "UI/UIBase.h"

class BossStatusUI : public IGameObject
{
public:
	BossStatusUI();
	~BossStatusUI();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	// --- 新しいUIシステムによる管理 ---
	UICanvas* m_canvas = nullptr;

	UIIcon* m_icon = nullptr;       // ボスの顔アイコン
	UIIcon* m_hpBarBack = nullptr;  // HPバーの背景枠
	UIGauge* m_hpBarGauge = nullptr;// HPバーのゲージ本体（可変）

private:
	bool m_isVisible = false;
	float m_displayHP = 0.0f;
};