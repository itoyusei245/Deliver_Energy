/**
 * @file CoinUI.cpp
 * @brief コインUIの実装
 */
#include "stdafx.h"
#include "CoinUI.h"
#include"GetItem.h"
#include"Game.h"

 /**
  * @brief コンストラクタ
  * @details 各スプライトの初期位置設定と、アイコンの読み込みを行います。
  */
CoinUI::CoinUI()
{
    Vector3 posIcon = { -900.0f,450.0f,0.0f };
    Vector3 posTens = { -850.0f,450.0f,0.0f };
    Vector3 posOnes = { -800.0f,450.0f,0.0f };

    // アイコンの初期化
    m_coinIcon.Init(COIN_ICON_TEX, 65.0f, 65.0f);
    m_coinIcon.SetPosition(posIcon);

    // 数字の配置座標を設定（テクスチャはUpdateで設定するためここでは位置のみ）
    m_digitTens.SetPosition(posTens);
    m_digitOnes.SetPosition(posOnes);

    // 初回更新を走らせるため、あり得ない値で初期化
    m_currentCount = -1;
}

CoinUI::~CoinUI()
{

}

/**
 * @brief 更新処理
 * @details
 * 1. GetItemからコイン数を取得し、上限（99枚）処理を行います。
 * 2. 前回から枚数が変わった場合のみ、10の位と1の位を計算し、
 * 対応する数字テクスチャ(0.DDS～9.DDS)を読み込み直します。
 */
void CoinUI::Update()
{
    // ゲームプレイ中のみ更新
    if (!Game::IsGamePlay) return;

    /**GetItemから現在の枚数を取得*/
    int count = GetItem::GetCoinCount();

    /**最大99枚で止める場合（安全策）*/
    if (count > 99) count = 99;

    /**枚数が変わった時だけ画像を更新（負荷軽減）*/
    if (m_currentCount != count)
    {
        m_currentCount = count;

        // 桁ごとの数値を算出
        int tens = m_currentCount / 10; // 10の位
        int ones = m_currentCount % 10; // 1の位

        /**10の位の更新*/
        if (tens > 0) {
            /**2桁（10枚以上）なら画像をセットして表示準備*/
            // NUM_TEX配列を使って、数字に対応したテクスチャをロード
            m_digitTens.Init(NUM_TEX[tens], 50.0f, 50.0f);
            m_digitTens.SetPosition(Vector3(-850.0f, 450.0f, 0.0f));
        }

        /**1の位の更新*/
        // 常に表示する
        m_digitOnes.Init(NUM_TEX[ones], 50.0f, 50.0f);
        m_digitOnes.SetPosition(Vector3(-800.0f, 450.0f, 0.0f));
    }

    /**スプライトの更新*/
    m_coinIcon.Update();

    // 10枚以上の場合のみ10の位を更新
    if (m_currentCount >= 10) m_digitTens.Update();

    m_digitOnes.Update();
}

/**
 * @brief 描画処理
 * @details 10枚未満の場合は10の位を描画しません。
 */
void CoinUI::Render(RenderContext& rc)
{
    if (!Game::IsGamePlay) return;

    m_coinIcon.Draw(rc);

    if (m_currentCount >= 10) {
        m_digitTens.Draw(rc); /**10枚以上なら10の位を描画*/
    }
    m_digitOnes.Draw(rc);
}