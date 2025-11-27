#pragma once

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();

    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender m_filterSprite; // 黒いフィルター画像
    bool m_isActive = false;     // 表示中かどうかのフラグ

    // 用意した半透明画像のパス
    const char* FILTER_TEX = "Assets/sprite/pause.DDS";
};