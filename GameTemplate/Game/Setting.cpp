/**
 * @file Setting.cpp
 * @brief 設定画面の実装
 */
#include "stdafx.h"
#include "Setting.h"
#include "Title.h"
#include "Sound/SoundManager.h"

namespace {
	constexpr float VOL_CHANGE_SPEED = 0.01f;
	constexpr float STICK_DEADZONE = 0.5f;
}

Setting::Setting()
{
	// UIの生成
	m_ui = new SettingUI();
	m_currentBar = 0;

	// 初期状態をUIにセット
	m_ui->SetCurrentBar(m_currentBar);
	m_ui->SetVolumes(
		SoundManager::Get().GetVolumeMaster(),
		SoundManager::Get().GetVolumeBGM(),
		SoundManager::Get().GetVolumeSE()
	);
}

Setting::~Setting()
{
	delete m_ui;
}

void Setting::Update()
{
	// 選択項目の移動処理
	UpdatePicUp();

	// 音量操作処理
	UpdateVolumeControl();

	// UIの更新
	m_ui->Update();

	// ボタン入力による効果音・終了処理
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		SoundManager::Get().PlaySE(enSoundKind_Decision);
	}
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		SoundManager::Get().PlaySE(enSoundKind_Decision);
		Title::IsSetting = false;
		DeleteGO(this);
	}
}

void Setting::UpdatePicUp()
{
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		SoundManager::Get().PlaySE(enSoundKind_Choose);
		m_currentBar--;
		if (m_currentBar < 0) {
			m_currentBar = 0;
		}
		m_ui->SetCurrentBar(m_currentBar);
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		SoundManager::Get().PlaySE(enSoundKind_Choose);
		m_currentBar++;
		if (m_currentBar > 3) {
			m_currentBar = 3;
		}
		m_ui->SetCurrentBar(m_currentBar);
	}
}

void Setting::UpdateVolumeControl()
{
	// 現在のSoundManagerの音量を取得
	float volMaster = SoundManager::Get().GetVolumeMaster();
	float volBGM = SoundManager::Get().GetVolumeBGM();
	float volSE = SoundManager::Get().GetVolumeSE();

	// 左スティックの左右入力
	float stickX = g_pad[0]->GetLStickXF();

	// カーソル位置に応じて音量を増減
	if (fabs(stickX) > STICK_DEADZONE)
	{
		if (m_currentBar == 0) {
			volMaster += stickX * VOL_CHANGE_SPEED;
		}
		else if (m_currentBar == 1) {
			volBGM += stickX * VOL_CHANGE_SPEED;
		}
		else if (m_currentBar == 2) {
			volSE += stickX * VOL_CHANGE_SPEED;
		}
	}

	volMaster = max(0.0f, min(volMaster, 1.0f));
	volBGM = max(0.0f, min(volBGM, 1.0f));
	volSE = max(0.0f, min(volSE, 1.0f));

	// SoundManagerに反映
	SoundManager::Get().SetVolumeMaster(volMaster);
	SoundManager::Get().SetVolumeBGM(volBGM);
	SoundManager::Get().SetVolumeSE(volSE);

	m_ui->SetVolumes(volMaster, volBGM, volSE);
}

void Setting::Render(RenderContext& rc)
{
	// 描画はすべてUIに任せる
	m_ui->Render(rc);
}