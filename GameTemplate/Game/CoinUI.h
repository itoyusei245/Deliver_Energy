/**
 * @file CoinUI.h
 * @brief コイン枚数表示UIのクラス定義
 */
#pragma once

 /**
  * @class CoinUI
  * @brief コインの所持数を表示するUIクラス
  * @details 画面左上に「コインアイコン」と「数字（2桁まで）」を表示します。
  * GetItemクラスから現在のコイン数を取得し、画像（スプライト）を切り替えて表示します。
  */
class CoinUI : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CoinUI();

	/**
	 * @brief デストラクタ
	 */
	~CoinUI();

	/**
	 * @brief 更新処理
	 * @details コイン数の変化を監視し、数字スプライトのテクスチャを更新します。
	 */
	void Update();

	/**
	 * @brief 描画処理
	 * @param[in,out] rc 描画コンテキスト
	 */
	void Render(RenderContext& rc);


private:
	/** コインのアイコン画像 */
	SpriteRender m_coinIcon;
	/** 10の位の数字画像 */
	SpriteRender m_digitTens;
	/** 1の位の数字画像 */
	SpriteRender m_digitOnes;

	/**
	 * @brief 現在表示中のコイン数
	 * @note 前回フレームの枚数と比較し、変化があった時のみ画像更新を行うために使用します。
	 */
	int m_currentCount = -1;

	/** コインアイコンのテクスチャパス */
	const char* COIN_ICON_TEX = "Assets/sprite/coin.DDS";

	/**
	 * @brief 数字用テクスチャパスの配列
	 * @details 0.DDS ～ 9.DDS までのパスを格納しています。
	 * インデックスがそのまま数字に対応します（例: NUM_TEX[3] は "3.DDS"）。
	 */
	const char* NUM_TEX[10] = {
		"Assets/sprite/0.DDS","Assets/sprite/1.DDS",
		"Assets/sprite/2.DDS","Assets/sprite/3.DDS",
		"Assets/sprite/4.DDS","Assets/sprite/5.DDS",
		"Assets/sprite/6.DDS","Assets/sprite/7.DDS",
		"Assets/sprite/8.DDS","Assets/sprite/9.DDS"
	};
};