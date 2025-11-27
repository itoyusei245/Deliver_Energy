#include "stdafx.h"
#include "GameTimeUI.h"
#include "Game.h"

GameTimeUI::GameTimeUI()
{
    //=== 座標設定 (画面右上を想定して並べる) ===
    /**左から順に: [十の位] [一の位] [区切り] [右数字] [セルシウス] */
    float baseY = 450.0f;
    Vector3 posTens  = { 720.0f, baseY, 0.0f }; /**10の位*/
    Vector3 posOnes  = { 770.0f, baseY, 0.0f }; /**1の位*/
    Vector3 posSep   = { 840.0f, baseY, 0.0f }; /**区切り(狭め)*/
    Vector3 posRight = { 870.0f, baseY, 0.0f }; /**右数字*/
    Vector3 posSuf   = { 920.0f, baseY, 0.0f }; 


    // === 初期化 ===
    m_spriteSuffix.Init(SUFFIX_TEX, 50.0f, 50.0f);
    m_spriteSuffix.SetPosition(posSuf);


    /**2. 区切り画像（固定）*/
    m_spriteSeparator.Init(SEP_TEX, 50.0f, 50.0f);
    m_spriteSeparator.SetPosition(posSep);


    /**3. 数字スプライト（位置だけセット、画像はUpdateで）*/
    m_spriteLeftTens.SetPosition(posTens);
    m_spriteLeftOnes.SetPosition(posOnes);
    m_spriteRight.SetPosition(posRight);


    m_timer = 0.0f;
}


GameTimeUI::~GameTimeUI()
{
}


void GameTimeUI::Update()
{
    if (!Game::IsGamePlay) return; 


    if (Game::IsPaused) return;

    m_timer += g_gameTime->GetFrameDeltaTime();


    // === 計算ロジック ===
    int totalCounts = (int)(m_timer / 5.0f);


    /**右側：0～9の繰り返し*/
    int valRight = totalCounts % 10;


    /**左側全体の値：初期値4 + 増加分*/
    /**※ % 10 を外したので、10, 11, 12...と無限に増えます */
    int valLeftTotal = 4 + (totalCounts / 10);


    /**左側を「桁」に分解*/
    int valLeftTens = valLeftTotal / 10; // 十の位 (例: 12なら1, 4なら0)
    int valLeftOnes = valLeftTotal % 10; // 一の位 (例: 12なら2, 4なら4)


    // === 画像更新処理 ===

    /**1. 左側：十の位の更新*/
    if (m_curLeftTens != valLeftTens)
    {
        m_curLeftTens = valLeftTens;
        if (m_curLeftTens > 0) {
            /**1以上なら表示*/
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