#include "stdafx.h"
#include "CoinUI.h"
#include"GetItem.h"
#include"Game.h"
CoinUI::CoinUI()
{
	Vector3 posIcon = { -900.0f,450.0f,0.0f };
	Vector3 posTens = { -850.0f,450.0f,0.0f };
	Vector3 posOnes = { -800.0f,450.0f,0.0f };


	m_coinIcon.Init(COIN_ICON_TEX, 65.0f, 65.0f);
	m_coinIcon.SetPosition(posIcon);


	m_digitTens.SetPosition(posTens);
	m_digitOnes.SetPosition(posOnes);


	m_currentCount = -1;
}
CoinUI::~CoinUI()
{

}
void CoinUI::Update()
{
    if (!Game::IsGamePlay) return;

    /**GetItemから現在の枚数を取得*/
    int count = GetItem::GetCoinCount();

    /**最大99枚で止める場合（安全策）*/
    if (count > 99) count = 99;

    /**枚数が変わった時だけ画像を更新*/
    if (m_currentCount != count)
    {
        m_currentCount = count;

        int tens = m_currentCount / 10;
        int ones = m_currentCount % 10;

        /**10の位の更新*/
        if (tens > 0) {
            /**2桁なら表示*/
            m_digitTens.Init(NUM_TEX[tens], 50.0f, 50.0f);
            m_digitTens.SetPosition(Vector3(-850.0f, 450.0f, 0.0f));
        }

        /**1の位の更新*/
        m_digitOnes.Init(NUM_TEX[ones], 50.0f, 50.0f);
        m_digitOnes.SetPosition(Vector3(-800.0f, 450.0f, 0.0f));
    }

    /**更新*/
    m_coinIcon.Update();
    if (m_currentCount >= 10) m_digitTens.Update(); /**10枚以上なら更新・表示*/
    m_digitOnes.Update();
}

void CoinUI::Render(RenderContext& rc)
{
    if (!Game::IsGamePlay) return;

    m_coinIcon.Draw(rc);

    if (m_currentCount >= 10) {
        m_digitTens.Draw(rc); /**10枚以上なら10の位を描画*/
    }
    m_digitOnes.Draw(rc);
}