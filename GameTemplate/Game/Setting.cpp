#include "stdafx.h"
#include "Setting.h"
#include "Game.h"
#include "Title.h"
#include "Sound/SoundManager.h"
/**
 * @brief Settingクラスのコンストラクタ
 * @details 設定画面用のスプライト画像を初期化します。
 */
Setting::Setting()
{
	/**設定画面の画像を読み込み*/
	m_settingSprite.Init("Assets/sprite/setting/Setting.DDS", 1920.0f, 1080.0f);
	m_picUpSprite[0].Init("Assets/sprite/setting/MasterVol.DDS", 1920.0f, 1080.0f);
	m_picUpSprite[1].Init("Assets/sprite/setting/BGMVol.DDS", 1920.0f, 1080.0f);
	m_picUpSprite[2].Init("Assets/sprite/setting/SEVol.DDS", 1920.0f, 1080.0f);
	m_picUpSprite[3].Init("Assets/sprite/setting/VoiceVol.DDS", 1920.0f, 1080.0f);
	

	float barX = -250.0f;	//バーの左端のX座標


	//マスター音量
	m_barMaster.Init("Assets/sprite/setting/MastarBar.DDS", 1000.0f, 20.0f);
	m_barMaster.SetPosition(Vector3(barX, 45.0f, 0.0f));
	m_barMaster.SetPivot({ 0.0f,0.5f });
	//BGM音量
	m_barBGM.Init("Assets/sprite/setting/BGMBar.DDS", 1000.0f,20.0f);
	m_barBGM.SetPosition(Vector3(barX, -13.0f, 0.0f));
	m_barBGM.SetPivot({ 0.0f,0.5f });
	//SE音量
	m_barSE.Init("Assets/sprite/setting/SEBar.DDS", 1000.0f, 20.0f);
	m_barSE.SetPosition(Vector3(barX, -71.0f, 0.0f));
	m_barSE.SetPivot({ 0.0f,0.5f });


	//枠組み
	m_barMasterFlame.Init("Assets/sprite/setting/MastarBarFlame.DDS", 1000.0f, 20.0f);
	m_barMasterFlame.SetPosition(Vector3(barX, 45.0f, 0.0f));
	m_barMasterFlame.SetPivot({ 0.0f,0.5f });

	m_barBGMFlame.Init("Assets/sprite/setting/BGMBarFlame.DDS", 1000.0f, 20.0f);
	m_barBGMFlame.SetPosition(Vector3(barX, -13.0f, 0.0f));
	m_barBGMFlame.SetPivot({ 0.0f,0.5f });

	m_barSEFlame.Init("Assets/sprite/setting/SEBarFlame.DDS", 1000.0f, 20.0f);
	m_barSEFlame.SetPosition(Vector3(barX, -71.0f, 0.0f));
	m_barSEFlame.SetPivot({ 0.0f,0.5f });
}

/**
 * @brief Settingクラスのデストラクタ
 */
Setting::~Setting()
{
}

/**
 * @brief 毎フレームの更新処理
 * @details 各音量の調整を行います
 */
void Setting::Update()
{
	UpdatePicUp();
	//音量操作処理
	UpdateVolumeControl();

	m_barMaster.Update();
	m_barBGM.Update();
	m_barSE.Update();
	m_barMasterFlame.Update();
	m_barBGMFlame.Update();
	m_barSEFlame.Update();

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

//音量操作の実装
void Setting::UpdateVolumeControl()
{
	//現在のSoundManagerの音量を取得
	float volMaster = SoundManager::Get().GetVolumeMastor();
	float volBGM = SoundManager::Get().GetVolumeBGM();
	float volSE = SoundManager::Get().GetVolumeSE();


	//左スティックの左右入力
	float stickX = g_pad[0]->GetLStickXF();


	//感度
	float speed = 0.01f;


	//カーソル位置に応じて音量を増減
	if (fabs(stickX) > 0.5f)
	{
		if (m_currentBar == 0) {
			//マスター音量
			volMaster += stickX * speed;
		}
		else if (m_currentBar == 1) {
			//BGM音量
			volBGM += stickX * speed;
		}
		else if (m_currentBar == 2) {
			//SE音量
			volSE += stickX * speed;
		}
	}


	//範囲を制限
	// 0.0 ～ 1.0 の範囲に制限 (Clamp)
	volMaster = max(0.0f, min(volMaster, 1.0f));
	volBGM = max(0.0f, min(volBGM, 1.0f));
	volSE = max(0.0f, min(volSE, 1.0f));

	// SoundManagerに反映
	SoundManager::Get().SetVolumeMaster(volMaster);
	SoundManager::Get().SetVolumeBGM(volBGM);
	SoundManager::Get().SetVolumeSE(volSE);

	// バーの長さを更新 (X方向のスケールで表現)
	m_barMaster.SetScale(Vector3(volMaster, 1.0f, 1.0f));
	m_barBGM.SetScale(Vector3(volBGM, 1.0f, 1.0f));
	m_barSE.SetScale(Vector3(volSE, 1.0f, 1.0f));
}

void Setting::UpdatePicUp()
{
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		SoundManager::Get().PlaySE(enSoundKind_Choose);
		if (m_currentBar == 0) {
			m_currentBar = 0;
		}
		else {
			m_currentBar = m_currentBar - 1.0f;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		SoundManager::Get().PlaySE(enSoundKind_Choose);
		if (m_currentBar == 3) {
			m_currentBar = 3;
		}
		else {
			m_currentBar = m_currentBar + 1.0f;
		}
	}
}

/**
 * @brief 設定画面の描画処理
 * @param rc 描画コンテキスト
 * @details 設定画面画像の描画を行います。
 */
void Setting::Render(RenderContext& rc)
{
	m_settingSprite.Draw(rc);
	m_picUpSprite[m_currentBar].Draw(rc);
	m_barMasterFlame.Draw(rc);
	m_barBGMFlame.Draw(rc);
	m_barSEFlame.Draw(rc);
	m_barMaster.Draw(rc);
	m_barBGM.Draw(rc);
	m_barSE.Draw(rc);
}
