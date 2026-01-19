/**
 * @file GameTimeUI.h
 * @brief ゲーム内経過時間（温度）表示UIのクラス定義
 */
#pragma once

 /**
  * @class GameTimeUI
  * @brief 経過時間に応じて上昇する「温度」を表示するUIクラス
  * @details
  * "4.0" からスタートし、時間経過とともに "4.1", "4.2"... と上昇していく数値を表示します。
  * （冷蔵庫のドアが開いている間の庫内温度上昇を表現していると思われます）
  * * 構成: [十の位][一の位] . [小数第一位] [単位アイコン]
  */
class GameTimeUI : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     */
    GameTimeUI();

    /**
     * @brief デストラクタ
     */
    ~GameTimeUI();

    /**
     * @brief 更新処理
     * @details タイマーを進め、5秒ごとに0.1ずつ数値を上昇させます。
     */
    void Update() override;

    /**
     * @brief 描画処理
     * @param[in,out] rc 描画コンテキスト
     */
    void Render(RenderContext& rc) override;

private:
    /** @name 表示用スプライト群 */
    /*@{*/
    SpriteRender m_spriteLeftTens; /**< 整数部の十の位 */
    SpriteRender m_spriteLeftOnes; /**< 整数部の一の位 */

    SpriteRender m_spriteSeparator;/**< 小数点（ドット） */

    SpriteRender m_spriteRight;    /**< 小数部の数字 */
    SpriteRender m_spriteSuffix;   /**< 末尾の単位・アイコン表示（timer.DDS） */
    /*@}*/

    /** 計測用タイマー */
    float m_timer = 0.0f;

    /** @name 現在の表示値キャッシュ
     * 無駄なテクスチャロード(Init)を防ぐために前回の値を保持します
     */
     /*@{*/
    int m_curLeftTens = -1; /**< 現在の十の位 */
    int m_curLeftOnes = -1; /**< 現在の一の位 */
    int m_curRight = -1;    /**< 現在の小数部 */
    /*@}*/

    /** 数字用テクスチャパス配列 (0.DDS ～ 9.DDS) */
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

    /** 末尾の単位画像パス */
    const char* SUFFIX_TEX = "Assets/sprite/timer.DDS";
    /** 区切り文字（ドット）パス */
    const char* SEP_TEX = "Assets/sprite/dot.DDS";
};