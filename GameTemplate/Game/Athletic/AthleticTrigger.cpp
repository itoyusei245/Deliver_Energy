/**
 * @file AthleticTrigger.cpp
 * @brief AthleticTriggerの実装
 */
#include "stdafx.h"
#include "AthleticTrigger.h"
#include "../StageManager.h"

namespace
{
	/**
	 *@brief レベルデータのファイルパス
	 */
	constexpr const char* LEVEL_DATA_PATH = "Assets/modelData/level/stageSpawn.tkl";

	/**
	 * @brief トリガーの定数
	 */
	constexpr const wchar_t* TRIGGER_ENEMY	  = L"enemySpawn";
	constexpr const wchar_t* TRIGGER_STAGE	  = L"stageSpawn";
	constexpr const wchar_t* TRIGGER_ATHLETIC = L"Athletic";

	/**
	 * @brief アスレチックギミックの定義
	 */
	const std::vector<AthleticSpawnInfo> ATHLETIC_GIMMICK_LIST = {
		// 普通の床
		{ "Assets/modelData/Stage/athletic_addStage.tkm", 0.0f, 0.0f },
		
		// 平行移動床
		{ "Assets/modelData/Stage/athletic_add_MoveFloor.tkm", 0.0f, 0.0f },
		
		/**
		 * 上下床
		 */
		{ "Assets/modelData/Stage/athletic_add_UpdownFloor.tkm",     0.0f,   0.0f },
		{ "Assets/modelData/Stage/athletic_add2_UpdownFloor1.tkm", 100.0f, 100.0f },
		{ "Assets/modelData/Stage/athletic_add2_UpdownFloor2.tkm", 100.0f, 125.0f },
		{ "Assets/modelData/Stage/athletic_add2_UpdownFloor3.tkm",  75.0f, 125.0f },
		{ "Assets/modelData/Stage/athletic_add2_UpdownFloor4.tkm", 100.0f, 100.0f },
		{ "Assets/modelData/Stage/athletic_add2_UpdownFloor5.tkm", 100.0f, 125.0f },
		{ "Assets/modelData/Stage/athletic_add3_UpdownFloor.tkm",  200.0f, 200.0f },

		/**
		 * 復帰床
		 */
		{ "Assets/modelData/Stage/athletic_add2_Return.tkm",     75.0f, 350.0f },
		{ "Assets/modelData/Stage/athletic_NormalMoveFloor.tkm",100.0f, 250.0f }
	};
}

AthleticTrigger::AthleticTrigger(){}

AthleticTrigger::~AthleticTrigger(){}

/**
 * @brief レベルデータのロードとオブジェクト生成
 * @details "Assets/modelData/level/stageSpawn.tkl" を読み込み、オブジェクト名に応じて以下の処理を行います。
 * - "enemySpawn": 敵発生用トリガーを作成
 * - "stageSpawn": ステージ発生用トリガーを作成
 * - "Athletic"  : アスレチック（動く床など）を作成。モデルパスや移動パラメータ（速度・範囲）を指定して生成します。
 */
bool AthleticTrigger::Start()
{
	// レベルデータを読み込む
	m_levelRender.Init(LEVEL_DATA_PATH,
		[&](LevelObjectData& objData)
		{
			// 敵出現用トリガーが見つかったら
			if (objData.EqualObjectName(TRIGGER_ENEMY))
			{
				// StageManagerへ敵出現ボックスの生成を依頼
				StageManager::GetInstance()->CreateEnemyBox(
					objData.position,
					objData.rotation,
					objData.scale
				);
				return true;
			}
			// ステージ出現用トリガーが見つかったら
			else if (objData.EqualObjectName(TRIGGER_STAGE))
			{
				// StageManagerへステージ出現ボックスの生成を依頼
				StageManager::GetInstance()->CreateStageBox(
					objData.position,
					objData.rotation,
					objData.scale
				);
				return true;
			}
			// アスレチック（動く床など）用トリガーが見つかったら
			else if (objData.EqualObjectName(TRIGGER_ATHLETIC))
			{
				// StageManagerにアスレチックギミックの生成を依頼
				// 第4引数に、生成するギミックの定義リスト（モデルパス, 移動速度, 移動範囲）を渡している
				StageManager::GetInstance()->CreateAthleticBox(
					objData.position,
					objData.rotation,
					objData.scale,
					ATHLETIC_GIMMICK_LIST
				);
				return true;
			}
			return true;
		});
	return true;
}