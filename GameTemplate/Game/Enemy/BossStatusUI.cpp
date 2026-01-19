/**
 * @file BossStatusUI.cpp
 * @brief ボスUIの実装
 */
#include "stdafx.h"
#include "BossStatusUI.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss.h"
#include "EnemyStatus.h"

BossStatusUI::BossStatusUI()
{
}

BossStatusUI::~BossStatusUI()
{
}

/**
 * @brief 開始処理
 * @details
 * - スプライト画像の読み込み
 * - ピボット（基準点）を(0,0)つまり左上に設定することで、スケール変更時に左側を固定して伸縮させます。
 * - 初期位置の設定
 */
bool BossStatusUI::Start()
{
	m_iconSprite.Init("Assets/sprite/Boss_HP_Icon.DDS", 100.0f, 100.0f);
	m_hpBarBack.Init("Assets/sprite/Boss_HP_Flame.DDS", 900.0f, 70.0f);
	m_hpBarGauge.Init("Assets/sprite/Boss_HP.DDS", 900.0f, 70.0f);

	// ピボットを左上(0.0, 0.0)に設定
	// これにより、スケールをX軸方向に縮小した際、左端を基準に右側が減っていく挙動になります。
	m_hpBarBack.SetPivot(Vector2(0.0f, 0.0f));
	m_hpBarGauge.SetPivot(Vector2(0.0f, 0.0f));

	m_iconSprite.SetPosition(ICON_POS);
	m_hpBarBack.SetPosition(BAR_POS);
	m_hpBarGauge.SetPosition(BAR_POS);

	// 表示用HPの初期値を適当な値（あるいはMAX）にしておく
	m_displayHP = 100.0f;

	return true;
}

/**
 * @brief 更新処理
 * @details
 * 1. EnemyManagerからボスのインスタンスを取得
 * 2. ボスがいる場合：
 * - 現在のHPと最大HPを取得
 * - 表示用HP(m_displayHP)を現在HP(currentHP)に向けて線形補間(Lerp)し、滑らかに減少させる
 * - HP割合(0.0～1.0)を算出し、ゲージスプライトのXスケールに反映
 * 3. ボスがいない場合：
 * - 非表示にする
 */
void BossStatusUI::Update()
{
	// シングルトンのマネージャからボスを取得
	Boss* boss = EnemyManager::GetInstance()->GetBoss();

	if (boss != nullptr) {

		// ボスがいる -> 表示フラグON
		m_isVisible = true;

		// ステータスを取得
		BossStatus* status = boss->GetStatus();
		float currentHP = (float)status->GetCurrentHP();
		float maxHP = (float)status->GetMaxHP();

		// --- HP減少アニメーション処理 ---
		float lerpSpeed = 0.1f;
		// 現在の実HPに向かって徐々に数値を近づける（イージング処理）
		m_displayHP += (currentHP - m_displayHP) * lerpSpeed;

		// 誤差が少なくなったら値を確定させる（ピタッと止めるため）
		if (abs(currentHP - m_displayHP) < 0.05f) {
			m_displayHP = currentHP;
		}

		// --- ゲージのスケール計算 ---
		float hpRatio = 1.0f;
		if (maxHP > 0.0f) {
			hpRatio = m_displayHP / maxHP;
		}

		// 0.0 ～ 1.0 の範囲にクランプ
		hpRatio = max(0.0f, min(hpRatio, 1.0f));

		// X軸のスケールにHP割合を適用してバーの長さを変える
		m_hpBarGauge.SetScale(Vector3(hpRatio, 1.0f, 1.0f));

		// スプライトの更新
		m_iconSprite.Update();
		m_hpBarBack.Update();
		m_hpBarGauge.Update();
	}
	else {
		// ボスがいない -> 表示フラグOFF
		m_isVisible = false;
	}
}

/**
 * @brief 描画処理
 */
void BossStatusUI::Render(RenderContext& rc)
{
	// フラグがtrueの場合のみ描画
	if (m_isVisible) {
		m_iconSprite.Draw(rc);
		m_hpBarBack.Draw(rc);
		m_hpBarGauge.Draw(rc);
	}
}