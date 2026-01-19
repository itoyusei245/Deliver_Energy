/**
 * @file BossStatusUI.h
 * @brief ボスのアイコンとHPバーを表示するUIクラス定義
 */
#pragma once
#include "BossStatusUI.h" // ※循環参照に見えますが、通常はIGameObject等の基底ヘッダを含めます

 /**
  * @class BossStatusUI
  * @brief ボスのHPとアイコンを表示・管理するUIクラス
  * @note EnemyManagerからボスの情報を取得し、HPの増減を滑らかにアニメーション表示します。
  * ボスが存在しない場合は非表示になります。
  */
class BossStatusUI : public IGameObject
{
public:
	BossStatusUI();
	~BossStatusUI();

	/**
	 * @brief 初期化処理
	 * @details スプライトのロード、ピボット設定（左詰め）、配置座標の設定を行います。
	 * @return bool 常にtrue
	 */
	bool Start() override;

	/**
	 * @brief 更新処理
	 * @details ボスの現在のHPを取得し、表示用HPを滑らかに追従させ（Lerp）、ゲージのスケールを更新します。
	 */
	void Update() override;

	/**
	 * @brief 描画処理
	 * @details ボスが存在する場合（m_isVisible == true）のみ描画を行います。
	 * @param[in,out] rc 描画コンテキスト
	 */
	void Render(RenderContext& rc) override;

private:
	/** ボスの顔アイコン画像 */
	SpriteRender m_iconSprite;
	/** HPバーの背景枠画像 */
	SpriteRender m_hpBarBack;
	/** HPバーのゲージ本体画像（可変） */
	SpriteRender m_hpBarGauge;

private:
	/**
	 * @brief 表示フラグ
	 * @note ボスが存在する間はtrue、倒されるかいない場合はfalseになります。
	 */
	bool m_isVisible = false;

	/** アイコンの配置座標 */
	const Vector3 ICON_POS = Vector3(-500.0f, 450.0f, 0.0f);
	/** HPバーの配置座標 */
	const Vector3 BAR_POS = Vector3(-400.0f, 450.0f, 0.0f);

	/**
	 * @brief 表示用のHP値
	 * @note 実際のHPが減った際、この値を徐々に近づけることでダメージ演出を滑らかにします。
	 */
	float m_displayHP = 0.0f;
};