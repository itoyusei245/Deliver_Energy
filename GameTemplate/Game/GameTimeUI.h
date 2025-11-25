#pragma once

class GameTimeUI : public IGameObject
{
public:
    GameTimeUI();
    ~GameTimeUI();

    void Update() override;
    void Render(RenderContext& rc) override;

private:
    // === 表示用のスプライト ===
    SpriteRender m_spriteLeftTens; // ★追加：左の数字（十の位）
    SpriteRender m_spriteLeftOnes; // ★変更：左の数字（一の位）

    SpriteRender m_spriteSeparator;// ★追加：真ん中の区切り（ドットなど）

    SpriteRender m_spriteRight;    // 右の数字
    SpriteRender m_spriteSuffix;   // 右端の固定表示

    // === 計測用 ===
    float m_timer = 0.0f;

    // === 現在の状態保存（無駄なInit防止用） ===
    int m_curLeftTens = -1; // 十の位
    int m_curLeftOnes = -1; // 一の位
    int m_curRight = -1; // 右の数字

    // === 画像パス ===
    const char* NUM_TEX[10] = {
        "Assets/sprite/0.DDS",
        "Assets/sprite/1.DDS",
        "Assets/sprite/2.DDS",
        "Assets/sprite/3.DDS",
        "Assets/sprite/4.DDS",
        "Assets/sprite/5.DDS",
        "Assets/sprite/6.DDS",
        "Assets/sprite/7.DDS",
        "Assets/sprite/8.DDS",
        "Assets/sprite/9.DDS"
    };

    const char* SUFFIX_TEX = "Assets/sprite/timer.DDS";
    // 区切り画像（ドットなど）
    const char* SEP_TEX = "Assets/sprite/dot.DDS";
};