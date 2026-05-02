/**
 * @file SettingUI.cpp
 * @brief 設定画面UIの実装
 */
#include "stdafx.h"
#include "SettingUI.h"

namespace {
	// --- 画像パス ---
	constexpr const char* PATH_SETTING_BG = "Assets/sprite/setting/Setting.DDS";

	constexpr const char* PATH_PICUP[4] = {
		"Assets/sprite/setting/MasterVol.DDS",
		"Assets/sprite/setting/BGMVol.DDS",
		"Assets/sprite/setting/SEVol.DDS",
		"Assets/sprite/setting/VoiceVol.DDS"
	};

	constexpr const char* PATH_BAR_MASTER_FRAME = "Assets/sprite/setting/MastarBarFlame.DDS";
	constexpr const char* PATH_BAR_BGM_FRAME = "Assets/sprite/setting/BGMBarFlame.DDS";
	constexpr const char* PATH_BAR_SE_FRAME = "Assets/sprite/setting/SEBarFlame.DDS";

	constexpr const char* PATH_BAR_MASTER = "Assets/sprite/setting/MastarBar.DDS";
	constexpr const char* PATH_BAR_BGM = "Assets/sprite/setting/BGMBar.DDS";
	constexpr const char* PATH_BAR_SE = "Assets/sprite/setting/SEBar.DDS";

	// --- サイズ・座標 ---
	const Vector2 SIZE_FULL(1920.0f, 1080.0f);
	const Vector3 POS_ZERO(0.0f, 0.0f, 0.0f);

	constexpr float BAR_WIDTH = 1000.0f;
	constexpr float BAR_HEIGHT = 20.0f;
	constexpr float BAR_START_X = -250.0f;

	const Vector3 POS_MASTER(BAR_START_X, 45.0f, 0.0f);
	const Vector3 POS_BGM(BAR_START_X, -13.0f, 0.0f);
	const Vector3 POS_SE(BAR_START_X, -71.0f, 0.0f);

	const Vector2 PIVOT_LEFT_CENTER(0.0f, 0.5f); // ゲージを左端から伸び縮みさせるためのピボット
}

SettingUI::SettingUI()
{
	m_canvas = new UICanvas();

	// 1. 背景
	m_settingBg = m_canvas->CreateUI<UIIcon>();
	m_settingBg->Initialize(PATH_SETTING_BG, SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

	// 2. ピックアップ枠
	m_picUpIcon = m_canvas->CreateUI<UIIcon>();
	m_picUpIcon->Initialize(PATH_PICUP[0], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);

	// 3. 枠線
	m_barMasterFrame = m_canvas->CreateUI<UIIcon>();
	m_barMasterFrame->Initialize(PATH_BAR_MASTER_FRAME, BAR_WIDTH, BAR_HEIGHT, POS_MASTER, Vector3::One, Quaternion::Identity);
	m_barMasterFrame->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);

	m_barBgmFrame = m_canvas->CreateUI<UIIcon>();
	m_barBgmFrame->Initialize(PATH_BAR_BGM_FRAME, BAR_WIDTH, BAR_HEIGHT, POS_BGM, Vector3::One, Quaternion::Identity);
	m_barBgmFrame->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);

	m_barSeFrame = m_canvas->CreateUI<UIIcon>();
	m_barSeFrame->Initialize(PATH_BAR_SE_FRAME, BAR_WIDTH, BAR_HEIGHT, POS_SE, Vector3::One, Quaternion::Identity);
	m_barSeFrame->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);

	// 4. ゲージ中身
	m_barMaster = m_canvas->CreateUI<UIGauge>();
	m_barMaster->Initialize(PATH_BAR_MASTER, BAR_WIDTH, BAR_HEIGHT, POS_MASTER, Vector3::One, Quaternion::Identity);
	m_barMaster->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);

	m_barBgm = m_canvas->CreateUI<UIGauge>();
	m_barBgm->Initialize(PATH_BAR_BGM, BAR_WIDTH, BAR_HEIGHT, POS_BGM, Vector3::One, Quaternion::Identity);
	m_barBgm->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);

	m_barSe = m_canvas->CreateUI<UIGauge>();
	m_barSe->Initialize(PATH_BAR_SE, BAR_WIDTH, BAR_HEIGHT, POS_SE, Vector3::One, Quaternion::Identity);
	m_barSe->GetSpriteRender()->SetPivot(PIVOT_LEFT_CENTER);
}

SettingUI::~SettingUI()
{
	delete m_canvas;
}

void SettingUI::Update()
{
	m_canvas->Update();
}

void SettingUI::Render(RenderContext& rc)
{
	m_canvas->Render(rc);
}

void SettingUI::SetCurrentBar(int index)
{
	// 変わった時だけ画像差し替え
	if (m_currentBarIndex != index && index >= 0 && index < 4) {
		m_currentBarIndex = index;
		m_picUpIcon->Initialize(PATH_PICUP[index], SIZE_FULL.x, SIZE_FULL.y, POS_ZERO, Vector3::One, Quaternion::Identity);
	}
}

void SettingUI::SetVolumes(float master, float bgm, float se)
{
	// UIGaugeの機能を使ってプログレス（0.0~1.0）を反映
	m_barMaster->SetProgress(master);
	m_barBgm->SetProgress(bgm);
	m_barSe->SetProgress(se);
}