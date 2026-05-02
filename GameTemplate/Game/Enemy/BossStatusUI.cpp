/**
 * @file BossStatusUI.cpp
 * @brief ボスUIの実装
 */
#include "stdafx.h"
#include "BossStatusUI.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss.h"
#include "EnemyStatus.h"
#include "Camera/EventCamera.h"

namespace
{
	constexpr const char* PATH_ICON		 = "Assets/sprite/Boss_HP_Icon.DDS";
	constexpr const char* PATH_BAR_BACK	 = "Assets/sprite/Boss_HP_Flame.DDS";
	constexpr const char* PATH_BAR_GAUGE = "Assets/sprite/Boss_HP.DDS";
	const Vector2		  SIZE_ICON		 = Vector2(100.0f, 100.0f);
	const Vector2		  SIZE_BAR		 = Vector2(900.0f, 70.0f);
	const Vector3		  POS_ICON		 = Vector3(-500.0f, 450.0f, 0.0f);
	const Vector3		  POS_BAR		 = Vector3(-400.0f, 450.0f, 0.0f);
	constexpr float		  LERP_SPEED	 = 0.1f;
	constexpr float		  SNAP_THRESHOLD = 0.05f;
}

BossStatusUI::BossStatusUI()
{
	// キャンバスの生成
	m_canvas = new UICanvas();

	// 各UIの生成
	m_icon = m_canvas->CreateUI<UIIcon>();
	m_hpBarBack = m_canvas->CreateUI<UIIcon>();
	m_hpBarGauge = m_canvas->CreateUI<UIGauge>();
}

BossStatusUI::~BossStatusUI()
{
	delete m_canvas;
}

bool BossStatusUI::Start()
{
	// アイコンの初期化
	m_icon->Initialize(PATH_ICON, SIZE_ICON.x, SIZE_ICON.y, POS_ICON, Vector3::One, Quaternion::Identity);

	// 背景枠の初期化
	m_hpBarBack->Initialize(PATH_BAR_BACK, SIZE_BAR.x, SIZE_BAR.y, POS_BAR, Vector3::One, Quaternion::Identity);
	// UIImageクラスが持っているGetSpriteRender()を使って、今まで通りピボットを調整
	m_hpBarBack->GetSpriteRender()->SetPivot(Vector2(0.0f, 0.0f));

	// ゲージ本体の初期化
	m_hpBarGauge->Initialize(PATH_BAR_GAUGE, SIZE_BAR.x, SIZE_BAR.y, POS_BAR, Vector3::One, Quaternion::Identity);
	m_hpBarGauge->GetSpriteRender()->SetPivot(Vector2(0.0f, 0.0f));

	m_displayHP = 100.0f;

	return true;
}

void BossStatusUI::Update()
{
	if (EnemyManager::GetInstance() == nullptr) return;

	Boss* boss = EnemyManager::GetInstance()->GetBoss();

	if (boss != nullptr) {
		m_isVisible = true;

		BossStatus* status = boss->GetStatus();
		float currentHP = (float)status->GetCurrentHP();
		float maxHP = (float)status->GetMaxHP();

		m_displayHP += (currentHP - m_displayHP) * LERP_SPEED;
		if (abs(currentHP - m_displayHP) < SNAP_THRESHOLD) {
			m_displayHP = currentHP;
		}

		float hpRatio = 1.0f;
		if (maxHP > 0.0f) {
			hpRatio = m_displayHP / maxHP;
		}

		m_hpBarGauge->SetProgress(hpRatio);

		// キャンバス全体を更新
		m_canvas->Update();
	}
	else {
		m_isVisible = false;
	}
}

void BossStatusUI::Render(RenderContext& rc)
{
	if (EventCamera::IsEventPlaying) return;

	if (m_isVisible) {
		m_canvas->Render(rc);
	}
}