#pragma once

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();

    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender m_filterSprite; /**黒いフィルター画像*/


    SpriteRender m_pauseSprite;


    SpriteRender m_selectSprute[4];


    bool m_isActive = false;     /**表示中かどうかのフラグ*/


    int m_selectBar = 0;


    /**用意した半透明画像のパス*/
    const char* FILTER_TEX = "Assets/sprite/pause.DDS";


    const char* TEX_PAESE = "Assets/sprite/pauseSelect.DDS";


    const char* TEX_SELECT[4] = {
        "Assets/sprite/pauseSelect_Continue.DDS",
        "Assets/sprite/pauseSelect_Retry.DDS",
        "Assets/sprite/pauseSelect_Setting.DDS",
        "Assets/sprite/pauseSelect_Title.DDS"
    };
};