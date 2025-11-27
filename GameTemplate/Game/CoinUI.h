#pragma once
class CoinUI:public IGameObject
{
public:
	CoinUI();
	~CoinUI();


	void Update();
	void Render(RenderContext& rc);


private:
	SpriteRender m_coinIcon;
	SpriteRender m_digitTens;
	SpriteRender m_digitOnes;


	int m_currentCount = -1;


	const char* COIN_ICON_TEX = "Assets/sprite/coin.DDS";
	const char* NUM_TEX[10] = {
		"Assets/sprite/0.DDS","Assets/sprite/1.DDS",
		"Assets/sprite/2.DDS","Assets/sprite/3.DDS",
		"Assets/sprite/4.DDS","Assets/sprite/5.DDS",
		"Assets/sprite/6.DDS","Assets/sprite/7.DDS",
		"Assets/sprite/8.DDS","Assets/sprite/9.DDS"
	};
};

