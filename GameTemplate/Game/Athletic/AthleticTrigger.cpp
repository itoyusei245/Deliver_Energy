#include "stdafx.h"
#include "AthleticTrigger.h"
#include "../StageManager.h"

AthleticTrigger::AthleticTrigger()
{
}

AthleticTrigger::~AthleticTrigger()
{
}

bool AthleticTrigger::Start()
{
	// レベルデータを読み込む
	m_levelRender.Init("Assets/modelData/level/stageSpawn.tkl",
		[&](LevelObjectData& objData)
		{
			// 敵出現用トリガーが見つかったら
			if (objData.EqualObjectName(L"enemySpawn"))
			{
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
				// StageManagerに生成を依頼
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

						// 4. 動かない床
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