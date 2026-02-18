/**
 * Type.h
 * サウンド用の定数など必要な情報を定義するファイル
 */
#pragma once
#include <string>


/** サウンドの種類 */
enum enSoundKind
{
	enSoundKind_SE = 0,
	enSoundKind_Decision = enSoundKind_SE,	//!< 決定SE
	//------------- SE --------------
	enSoundKind_Choose,						//!< 選択音
	enSoundKind_Refrigerator,				//!< 冷蔵庫音
	enSoundKind_Jump,						//!< ジャンプ音
	enSoundKind_Roll,						//!< 回転音
	enSoundKind_EnemyCount,					//!< ボス出現音
	enSoundKind_EnemyVoice,					//!< ボスの咆哮音
	enSoundKind_DieEnemy,					//!< 敵撃破音
	enSoundKind_DieBoss,					//!< ボス撃破音
	enSoundKind_Landing,					//!< 着地音
	enSoundKind_Walk,						//!< 移動音
	enSoundKind_ScoreCount,					//!< カウント音
	enSoundKind_CountUp,					//!< カウント結果音
	enSoundKind_Pause,						//!< 一時停止音
	enSoundKind_Coin,						//!< コイン獲得音
	enSoundKind_RefrigeratorDoor,			//!< 冷蔵庫の扉音
	enSoundKind_SE_Max,						//!< SEの最大数
	enSoundKind_BGM = enSoundKind_SE_Max,
	enSoundKind_Title = enSoundKind_BGM,	//!< タイトルBGM
	//------------- BGM --------------
	enSoundKind_Normal,						//!< 通常ステージBGM
	enSoundKind_Boss,						//!< ボス戦BGM
	enSoundKind_Athletic_01,				//!< アスレチックステージBGM1
	enSoundKind_Athletic_02,				//!< アスレチックステージBGM2
	enSoundKind_Result,						//!< リザルトBGM
	enSoundKind_Max,						//!< BGMの最大数
	enSoundKind_None = enSoundKind_Max,
};


/** サウンドの情報の構造体 */
struct SoundInformation
{
	std::string assetPath;
	//
	SoundInformation(const std::string& path) : assetPath(path) {}
};


/**
 *	情報を保持
 *	流すsoundファイルをSoundInformationに格納
 */
static SoundInformation soundInformation[enSoundKind_Max] =
{
	// SE
	SoundInformation("Assets/Sound/Select.wav"),			//!< 決定SE
	SoundInformation("Assets/Sound/Choose.wav"),			//!< 選択SE	
	SoundInformation("Assets/Sound/Refrigerator.wav"),		//!< 冷蔵庫SE
	SoundInformation("Assets/Sound/Jump.wav"),				//!< ジャンプSE
	SoundInformation("Assets/Sound/Roll.wav"),				//!< 回転SE
	SoundInformation("Assets/Sound/EnemyCountDown.wav"),	//!< ボス出現SE
	SoundInformation("Assets/Sound/EnemyVoice.wav"),		//!< ボスの咆哮SE
	SoundInformation("Assets/Sound/DieEnemy.wav"),			//!< 敵撃破SE
	SoundInformation("Assets/Sound/DieBoss.wav"),			//!< ボス撃破SE
	SoundInformation("Assets/Sound/Landing.wav"),			//!< 着地SE
	SoundInformation("Assets/Sound/Walk.wav"),				//!< 移動SE
	SoundInformation("Assets/Sound/ScoreCount.wav"),		//!< カウントSE
	SoundInformation("Assets/Sound/CountUp.wav"),			//!< カウント結果SE
	SoundInformation("Assets/Sound/Pause.wav"),				//!< 一時停止SE
	SoundInformation("Assets/Sound/Coin.wav"),				//!< コイン獲得SE
	SoundInformation("Assets/Sound/Refrigerator_Open.wav"),	//!< 冷蔵庫の扉SE
	//BGM
	SoundInformation("Assets/Sound/Title.wav"),				//!< タイトルBGM
	SoundInformation("Assets/Sound/Normal.wav"),			//!< 通常ステージBGM
	SoundInformation("Assets/Sound/Boss.wav"),				//!< ボス戦BGM
	SoundInformation("Assets/Sound/Athletic01.wav"),		//!< アスレチックステージBGM1
	SoundInformation("Assets/Sound/Athletic02.wav"),		//!< アスレチックステージBGM2
	SoundInformation("Assets/Sound/Result.wav")				//!< リザルトBGM
};
