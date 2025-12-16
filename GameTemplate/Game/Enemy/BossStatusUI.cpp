#include "stdafx.h"
#include "BossStatusUI.h"
#include "EnemyManager.h"
#include "Enemy/Boss.h"
#include "EnemyStatus.h"

BossStatusUI::BossStatusUI()
{
}

BossStatusUI::~BossStatusUI()
{
}

bool BossStatusUI::Start()
{
	m_iconSprite.Init("Assets/sprite/Boss_HP_Icon.DDS", 100.0f, 100.0f);
	m_hpBarBack.Init("Assets/sprite/Boss_HP_Flame.DDS", 900.0f, 70.0f);
	m_hpBarGauge.Init("Assets/sprite/Boss_HP.DDS", 900.0f, 70.0f);

	m_hpBarBack.SetPivot(Vector2(0.0f, 0.0f));
	m_hpBarGauge.SetPivot(Vector2(0.0f, 0.0f));

	m_iconSprite.SetPosition(ICON_POS);
	m_hpBarBack.SetPosition(BAR_POS);
	m_hpBarGauge.SetPosition(BAR_POS);

	m_displayHP = 100.0f;

	return true;
}

void BossStatusUI::Update()
{
	Boss* boss = EnemyManager::GetInstance()->GetBoss();

	if (boss != nullptr) {

		//ボスがいる->フラグON
		m_isVisible = true;


		//ステータスを取得
		BossStatus* status = boss->GetStatus();
		float currentHP = (float)status->GetCurrentHP();
		float maxHP = (float)status->GetMaxHP();

		float lerpSpeed = 0.1f;
		m_displayHP += (currentHP - m_displayHP) * lerpSpeed;

		if (abs(currentHP - m_displayHP) < 0.05f) {
			m_displayHP = currentHP;
		}

		float hpRatio = 1.0f;
		if (maxHP > 0.0f) {
			hpRatio = m_displayHP / maxHP;
		}

		hpRatio = max(0.0f, min(hpRatio, 1.0f));

		m_hpBarGauge.SetScale(Vector3(hpRatio, 1.0f, 1.0f));

		m_iconSprite.Update();
		m_hpBarBack.Update();
		m_hpBarGauge.Update();
	}
	else{
		//ボスがいない->フラグOFF
		m_isVisible = false;
	}
}

void BossStatusUI::Render(RenderContext& rc)
{
	//フラグがtrueの場合のみ描画
	if (m_isVisible) {
		m_iconSprite.Draw(rc);
		m_hpBarBack.Draw(rc);
		m_hpBarGauge.Draw(rc);
	}
}