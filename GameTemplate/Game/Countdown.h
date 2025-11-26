#pragma once

class Countdown : public IGameObject
{
public:
    Countdown();
    ~Countdown();

    void Update() override;
    void Render(RenderContext& rc) override;

    /**カウントダウンが終わったかどうか*/
    bool IsFinished() const { return m_isFinished; }

private:
    SpriteRender m_spriteRender; /**画像表示用*/
    float m_timer = 0.0f;        /**経過時間タイマー*/
    bool m_isFinished = false;   /**終了フラグ*/

    /**画像のパス（適宜変更してください）*/
    const char* TEX_READY = "Assets/sprite/countDown_Ready.DDS";
    const char* TEX_5 = "Assets/sprite/5.DDS";
    const char* TEX_4 = "Assets/sprite/4.DDS";
    const char* TEX_3 = "Assets/sprite/3.DDS";
    const char* TEX_2 = "Assets/sprite/2.DDS";
    const char* TEX_1 = "Assets/sprite/1.DDS";
    const char* TEX_GO = "Assets/sprite/countDown_GO.DDS";
};