/**
 * @file AthleticManager.cpp
 * @brief アスレチックマネージャーの実装
 */
#include "stdafx.h"
#include "Athletic/AthleticManager.h"
#include "Athletic/AthleticTrigger.h"

 // 静的メンバ変数の初期化
AthleticManager* AthleticManager::m_instance = nullptr;

AthleticManager::AthleticManager()
{
}

AthleticManager::~AthleticManager()
{
}

/**
 * @brief 初期化処理
 * @details トリガーローダー(AthleticTrigger)を生成し、監視を開始させます。
 */
void AthleticManager::Setup()
{
	NewGO<AthleticTrigger>(0, "athleticTriggerLoader");
}

/**
 * @brief トリガー用コリジョンの生成
 * @details 既にトリガーが存在する場合は生成しません。Box形状のコリジョンを作成します。
 */
void AthleticManager::CreateStageTrigger(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	if (m_stageTrigger != nullptr) return;

	m_stageTrigger = NewGO<CollisionObject>(0);
	m_stageTrigger->CreateBox(pos, rot, scale);
	m_stageTrigger->SetName("stageTrigger");
}

/**
 * @brief トリガー用コリジョンの削除
 * @details メモリリークを防ぐため、存在確認を行ってから削除します。
 */
void AthleticManager::DeleteStageTrigger()
{
	if (m_stageTrigger) {
		DeleteGO(m_stageTrigger);
		m_stageTrigger = nullptr;
	}
}

/**
 * @brief アスレチックステージのスポーン
 * @details ステージが未生成の場合のみ、AthleticStageクラスをインスタンス化します。
 */
void AthleticManager::SpawnAthleticStage()
{
	if (m_currentStage != nullptr) return;
	m_currentStage = NewGO<AthleticStage>(0, "athleticStage");
}