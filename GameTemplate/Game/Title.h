#pragma once
class Title : public IGameObject
{
public:
    Title();
    ~Title();


    bool Start();


    void Update();


    void Render(RenderContext& rc);


    /** タイトル画面かどうか */
    static bool IsTitle;


    /** 設定画面を開いているかどうか */
    static bool IsSetting;


    SpriteRender m_titleSprite;
    SpriteRender m_picUpSprite[3];

private:
    int m_currentBar = 0;

    void UpdatePicUp();
};
