#pragma once
class Title : public IGameObject
{
public:
    Title();
    ~Title();


    void Update();


    void Render(RenderContext& rc);


    SpriteRender m_titleSprite;
    SpriteRender m_picUpSprite[3];

private:
    int m_currentBar = 0;

    void UpdatePicUp();
};
