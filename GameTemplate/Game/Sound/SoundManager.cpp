/**
 * SoundManager.cpp
 * サウンド管理
 * 必要なサウンドファイルを読み込んだり再生したりなど管理する
 */
#include "stdafx.h"
#include "SoundManager.h"


SoundManager* SoundManager::instance_ = nullptr;//初期化


SoundManager::SoundManager()
{
	// リストの削除
	seList_.clear();

	// サウンドの登録
	for (int i = 0; i < ARRAYSIZE(soundInformation); ++i) {
		const auto& info = soundInformation[i];
		g_soundEngine->ResistWaveFileBank(i, info.assetPath.c_str());
	}
}

SoundManager::~SoundManager()
{
}


void SoundManager::Update()
{
	// SEリストから再生していないものがあれば削除する
	std::vector<SoundHandle>eraseList;      
	for (auto& it : seList_) {
		const auto key = it.first;
		auto* se = it.second;
		// 再生が終わっているなら削除
		if (!se->IsPlaying())
		{
			DeleteGO(se);
			eraseList.push_back(key);
		}
	}
	for (const auto& key : eraseList) {
		seList_.erase(key);
	}
}


void SoundManager::PlayBGM(const int kind)
{
	// BGMが生成されていない
	if (bgm_ == nullptr) {
		//生成
		bgm_ = NewGO<SoundSource>(0, "bgm");
	}
	else {
		// すでに生成されているならBGMを停止する
		bgm_->Stop();
	}
	//初期化
	bgm_->Init(kind);								// BGMの初期化
	bgm_->SetVolume(volumeMaster_ * volumeBGM_);	//再生開始時に音量を適用(Master * BGM)
	bgm_->Play(true);								// BGMなのでループ再生する
}


void SoundManager::StopBGM()
{
	if (bgm_ == nullptr) {
		return;
	}
	bgm_->Stop();
}


SoundHandle SoundManager::PlaySE(const int kind, const bool isLood, const bool is3D)
{
	// ハンドルが最大数になったら使えない
	// NOTE: そんなに再生するはずがない
	if (soundHandleCount_ == INVALID_SOUND_HANDLE) {
		K2_ASSERT(false, "サウンドの再生が多いです。\n");
		return INVALID_SOUND_HANDLE;
	}
	auto* se = NewGO<SoundSource>(0, "se");
	se->Init(kind, is3D);						// SEの初期化
	se->SetVolume(volumeMaster_ * volumeSE_);	//再生開始時に音量を適用(Master * SE)
	se->Play(isLood);						

	seList_.emplace(++soundHandleCount_, se);

	return soundHandleCount_;
}


void SoundManager::StopSE(const SoundHandle handle)
{
	auto* se = FindSE(handle);
	if (se == nullptr) {
		return;
	}
	se->Stop();
}


void SoundManager::SetVolumeMaster(float vol)
{
	volumeMaster_ = vol;
	//BGMに適用
	if (bgm_ != nullptr) {
		bgm_->SetVolume(volumeMaster_ * volumeBGM_);
	}
	//SEに適用
	for (auto& pair : seList_) {
		pair.second->SetVolume(volumeMaster_ * volumeSE_);
	}
}


void SoundManager::SetVolumeBGM(float vol)
{
	volumeBGM_ = vol;
	//BGMに適用
	if (bgm_ != nullptr) {
		bgm_->SetVolume(volumeMaster_ * volumeBGM_);
	}
}


void SoundManager::SetVolumeSE(float vol)
{
	volumeSE_ = vol;
	//全ての再生中のSEに適用
	for (auto& pair : seList_) {
		pair.second->SetVolume(volumeMaster_ * volumeSE_);
	}
}