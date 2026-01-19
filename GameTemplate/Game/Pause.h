/**
 * @file Pause.h
 * @brief ポーズ画面クラス定義
 */
#pragma once

 /**
  * @class Pause
  * @brief ゲーム内ポーズ機能を提供するクラス
  * @details スタートボタンで表示切替を行い、ゲームの進行を停止させます。
  * メニュー（再開、リトライ、タイトルへ戻る）の選択処理も担います。
  */
class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();

    void Update();
    void Render(RenderContext& rc);

private:
    /** 背景を暗くするフィルター画像 */
    SpriteRender m_filterSprite;

    /** "PAUSE" 文字画像 */
    SpriteRender m_pauseSprite;

    /** 選択肢（Continue, Retry, Setting, Title）の画像配列 */
    SpriteRender m_selectSprute[4];

    /** 表示中（ポーズ中）かどうかのフラグ */
    bool m_isActive = false;

    /** 現在選択中のメニュー番号（0～3） */
    int m_selectBar = 0;

    // --- 画像パス定数 ---
    const char* FILTER_TEX = "Assets/sprite/pause.DDS";
    const char* TEX_PAESE = "Assets/sprite/pauseSelect.DDS";

    const char* TEX_SELECT[4] = {
        "Assets/sprite/pauseSelect_Continue.DDS",
        "Assets/sprite/pauseSelect_Retry.DDS",
        "Assets/sprite/pauseSelect_Setting.DDS",
        "Assets/sprite/pauseSelect_Title.DDS"
    };
};