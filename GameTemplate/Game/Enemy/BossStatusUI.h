/**
 *BossStatusUI.h
 *ボスのアイコンとHPを表示するクラス
 */
#pragma once
#include "BossStatusUI.h"


class BossStatusUI:public IGameObject
{
public:
	BossStatusUI();
	~BossStatusUI();


	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


private:
	SpriteRender m_iconSprite;
	SpriteRender m_hpBarBack;
	SpriteRender m_hpBarGauge;


private:
	//表示フラグ
	bool m_isVisible=false;
	const Vector3 ICON_POS = Vector3(-500.0f, 450.0f, 0.0f);
	const Vector3 BAR_POS = Vector3(-400.0f, 450.0f, 0.0f);
	float m_displayHP = 0.0f;
};