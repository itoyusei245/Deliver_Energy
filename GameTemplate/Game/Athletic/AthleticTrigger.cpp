/**
 * @file AthleticTrigger.cpp
 * @brief AthleticTriggerの実装
 */
#include "stdafx.h"
#include "AthleticTrigger.h"
#include "../StageManager.h" // StageManagerへの参照

AthleticTrigger::AthleticTrigger()
{
}

AthleticTrigger::~AthleticTrigger()
{
}

/**
 * @brief レベルデータのロードとオブジェクト生成
 * @details "Assets/modelData/level/stageSpawn.tkl" を読み込み、オブジェクト名に応じて以下の処理を行います。
 * - "enemySpawn": 敵発生用トリガーを作成
 * - "stageSpawn": ステージ発生用トリガーを作成
 * - "Athletic"  : アスレチック（動く床など）を作成。モデルパスや移動パラメータ（速度・範囲）を指定して生成します。
 * @return bool 常にtrueを返します。
 */
bool AthleticTrigger::Start()
{
	// レベルデータを読み込む
	m_levelRender.Init("Assets/modelData/level/stageSpawn.tkl",
		[&](LevelObjectData& objData)
		{
			// 敵出現用トリガーが見つかったら
			if (objData.EqualObjectName(L"enemySpawn"))
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
			else if (objData.EqualObjectName(L"stageSpawn"))
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
			else if (objData.EqualObjectName(L"Athletic"))
			{
				// StageManagerにアスレチックギミックの生成を依頼
				// 第4引数に、生成するギミックの定義リスト（モデルパス, 移動速度, 移動範囲）を渡している
				StageManager::GetInstance()->CreateAthleticBox(
					objData.position,
					objData.rotation,
					objData.scale,
					{
						// { "モデルのファイルパス", 移動速度, 移動範囲 }

						// 1. 普通の床（動かない）
						{ "Assets/modelData/Stage/athletic_NormalMoveFloor.tkm", 0.0f, 0.0f },

						// 2. 上下に動く床 (速度100.0f, 範囲150.0f)
						{ "Assets/modelData/Stage/athletic_add_MoveFloor.tkm", 100.0f, 150.0f },

						// 3. 上下に動く床 (速度を遅く、範囲を大きく)
						{ "Assets/modelData/Stage/athletic_add_UpdownFloor.tkm", 50.0f, 300.0f },

						// 4. 動かない床、または固有の動きを持つ床の定義
						{ "Assets/modelData/Stage/athletic_addStage.tkm", 0.0f, 0.0f },
						{ "Assets/modelData/Stage/athletic_add2_UpdownFloor1.tkm", 100.0f, 200.0f },
						{ "Assets/modelData/Stage/athletic_add2_UpdownFloor2.tkm", 120.0f, 200.0f },
						{ "Assets/modelData/Stage/athletic_add2_UpdownFloor3.tkm", 140.0f, 200.0f },
						{ "Assets/modelData/Stage/athletic_add2_UpdownFloor4.tkm", 160.0f, 200.0f },
						{ "Assets/modelData/Stage/athletic_add2_UpdownFloor5.tkm", 180.0f, 200.0f },
						{ "Assets/modelData/Stage/athletic_add3_UpdownFloor.tkm", 80.0f, 150.0f },
						{ "Assets/modelData/Stage/athletic_add2_Return.tkm", 0.0f, 0.0f }
					}
				);
				return true;
			}
			return true;
		});
	return true;
}