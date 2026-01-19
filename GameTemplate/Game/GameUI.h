#pragma once
#include "Player.h" 

class GameUI : public IGameObject
{
public:
    GameUI();  // コンストラクタ
    ~GameUI(); // デストラクタ

    void Update();
    void Render(RenderContext& rc);

    // Titleクラスと同じようにpublicでSpriteRenderを持つ
    SpriteRender m_hpGauge;

    // HPゲージの枠（フレーム）
    SpriteRender m_hpFrame;

    // プレイヤーをセットするための関数
    void SetPlayer(Player* player) {
        m_player = player;
    }

private:
    Player* m_player = nullptr;

    // CircleInitに渡すための変数
    float m_gaugeProgress = 1.0f; // ゲージの進行度
    float m_gaugeSize = 1.0f;     // ゲージのサイズ
};