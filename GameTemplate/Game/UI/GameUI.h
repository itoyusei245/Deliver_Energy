/**
 * @file GameUI.h
 * @brief ゲームプレイ中のUI（HPゲージなど）を管理するクラス
 */
#pragma once
#include "Player.h" 
#include "UIBase.h" 

class GameUI : public IGameObject
{
public:
    GameUI();  // コンストラクタ
    ~GameUI(); // デストラクタ

    void Update() override;
    void Render(RenderContext& rc) override;

    // プレイヤーをセットするための関数
    void SetPlayer(Player* player) {
        m_player = player;
    }

private:
    Player* m_player = nullptr;
    UICanvas* m_canvas = nullptr;  // UIをまとめるキャンバス
    UIIcon* m_hpFrame = nullptr;   // HPゲージの枠（ただの画像として扱うのでIcon）
    UIGauge* m_hpGauge = nullptr;  // HPゲージ本体（円形ゲージ対応版）
};