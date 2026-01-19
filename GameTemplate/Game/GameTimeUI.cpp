/**
 * @file GameTimeUI.cpp
 * @brief ゲームタイム（温度）UIの実装
 */
#include "stdafx.h"
#include "GameTimeUI.h"
#include "Game.h"

 /**
  * @brief コンストラクタ
  * @details 画面右上に [数字][.][数字][単位] の順でスプライトを配置・初期化します。
  */
GameTimeUI::GameTimeUI()
{
    //=== 座標設定 (画面右上を想定して並べる) ===
    /**左から順に: [十の位] [一の位] [区切り] [右数字] [セルシウス] */
    float baseY = 450.0f;
    Vector3 posTens = { 720.0f, baseY, 0.0f }; /**10の位*/
    Vector3 posOnes = { 770.0f, baseY, 0.0f }; /**1の位*/
    Vector3 posSep = { 840.0f, baseY, 0.0f }; /**区切り(狭め)*/
    Vector3 posRight = { 870.0f, baseY, 0.0f }; /**右数字*/
    Vector3 posSuf = { 920.0f, baseY, 0.0f };


    // === 初期化 ===
    // 単位（℃などを想定）の表示
    m_spriteSuffix.Init(SUFFIX_TEX, 50.0f, 50.0f);
    m_spriteSuffix.SetPosition(posSuf);


    /**2. 区切り画像（固定ドット）*/
    m_spriteSeparator.Init(SEP_TEX, 50.0f, 50.0f);
    m_spriteSeparator.SetPosition(posSep);


    /**3. 数字スプライト（位置だけセット、画像はUpdateで値が決まり次第ロード）*/
    m_spriteLeftTens.SetPosition(posTens);
    m_spriteLeftOnes.SetPosition(posOnes);
    m_spriteRight.SetPosition(posRight);

    m_timer = 0.0f;
}


GameTimeUI::~GameTimeUI()
{
}

/**
 * @brief 更新処理
 * @details
 * - ゲームプレイ中のみタイマーを進めます。
 * - 5.0秒経過するごとにカウントが1つ増えます。
 * - 表示ロジック：
 * - 小数部（右）：カウント % 10 (0.1刻みで増える演出)
 * - 整数部（左）：4 + (カウント / 10) (初期値4からスタート)
 * - 例：0秒 -> 4.0, 5秒 -> 4.1, 50秒 -> 5.0
 */
void GameTimeUI::Update()
{
    // ゲームプレイ中でなければ更新しない
    if (!Game::IsGamePlay) return;

    // ポーズ中も更新しない
    if (Game::IsPaused) return;

    m_timer += g_gameTime->GetFrameDeltaTime();


    // === 計算ロジック ===
    // 5秒で1カウント進む
    int totalCounts = (int)(m_timer / 5.0f);


    /**右側（小数第一位）：0～9の繰り返し*/
    int valRight = totalCounts % 10;


    /**左側全体（整数部）の値：初期値4 + 10カウント（50秒）ごとに1上昇*/
    int valLeftTotal = 4 + (totalCounts / 10);


    /**左側を「桁」に分解*/
    int valLeftTens = valLeftTotal / 10; // 十の位 (例: 12なら1, 4なら0)
    int valLeftOnes = valLeftTotal % 10; // 一の位 (例: 12なら2, 4なら4)


    // === 画像更新処理 ===
    // 前回と値が変わった場合のみ Init を呼んでテクスチャを差し替える

    /**1. 左側：十の位の更新*/
    if (m_curLeftTens != valLeftTens)
    {
        m_curLeftTens = valLeftTens;
        if (m_curLeftTens > 0) {
            /**1以上なら表示準備*/
            m_spriteLeftTens.Init(NUM_TEX[m_curLeftTens % 10], 50.0f, 50.0f);
            m_spriteLeftTens.SetPosition(Vector3(760.0f, 450.0f, 0.0f));
        }
    }


    /**2.左側：一の位の更新*/
    if (m_curLeftOnes != valLeftOnes)
    {
        m_curLeftOnes = valLeftOnes;
        m_spriteLeftOnes.Init(NUM_TEX[m_curLeftOnes], 50.0f, 50.0f);
        m_spriteLeftOnes.SetPosition(Vector3(810.0f, 450.0f, 0.0f));
    }


    /**3.右側の更新*/
    if (m_curRight != valRight)
    {
        m_curRight = valRight;
        m_spriteRight.Init(NUM_TEX[m_curRight], 50.0f, 50.0f);
        m_spriteRight.SetPosition(Vector3(870.0f, 450.0f, 0.0f));
    }


    // === 更新反映 ===
    if (m_curLeftTens > 0) m_spriteLeftTens.Update(); /**0より大きい時だけ更新*/
    m_spriteLeftOnes.Update();
    m_spriteSeparator.Update();
    m_spriteRight.Update();
    m_spriteSuffix.Update();
}

/**
 * @brief 描画処理
 */
void GameTimeUI::Render(RenderContext& rc)
{
    if (!Game::IsGamePlay) return;


    /**10の位は 0 のときは描画しない（4.0 のときに 04.0 にならないように）*/
    if (m_curLeftTens > 0) {
        m_spriteLeftTens.Draw(rc);
    }

    m_spriteLeftOnes.Draw(rc);
    m_spriteSeparator.Draw(rc);
    m_spriteRight.Draw(rc);
    m_spriteSuffix.Draw(rc);
}