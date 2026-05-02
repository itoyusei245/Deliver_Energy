/**
 * @file CoinUI.h
 * @brief コイン枚数表示UIのクラス定義
 */
#pragma once
#include "UIBase.h"

class CoinUI : public IGameObject
{
public:
	CoinUI();
	~CoinUI();

	void Update() override;
	void Render(RenderContext& rc) override;

private:
	UICanvas* m_canvas = nullptr;
	UIIcon* m_coinIcon = nullptr;   // 左側のコインアイコン
	UINumber* m_coinNumber = nullptr; // 右側の数字（桁数自動）
};