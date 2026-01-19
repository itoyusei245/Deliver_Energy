/**
 * @file AthleticTrigger.h
 * @brief レベルデータ読み込み用クラス定義
 */
#pragma once

 /**
  * @class AthleticTrigger
  * @brief ステージ上のトリガーやギミックを生成するクラス
  * @note LevelRenderを使用して.tklファイルを読み込み、配置されたオブジェクト名に応じて
  * StageManagerに対し生成命令を発行します。
  */
class AthleticTrigger : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	 */
	AthleticTrigger();

	/**
	 * @brief デストラクタ
	 */
	~AthleticTrigger();

	/**
	 * @brief 初期化・ロード処理
	 * @details レベルデータ(.tkl)を読み込み、各種オブジェクトの生成を行います。
	 * @return bool 成功時true
	 */
	bool Start() override;

	/**
	 * @brief 更新処理
	 * @details このクラスはロードが主目的のため、更新処理は行いません。
	 */
	void Update() override {}

private:
	/**
	 * @brief レベルデータ描画・読み込み用オブジェクト
	 */
	LevelRender m_levelRender;
};