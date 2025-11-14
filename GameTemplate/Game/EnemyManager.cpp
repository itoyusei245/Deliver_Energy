/**“EnemyManager”敵キャラクターの管理を行うシングルトンパターンを用いたクラス*/
#include "stdafx.h"
#include "EnemyManager.h"
#include "BossEnemy.h"
#include "StageManager.h"
#include "Player.h"

EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}
/**敵マネージャーの更新メソッド*/
void EnemyManager::Update()
{
    // プレイヤー取得
    Player* player = FindGO<Player>("player");
    if (!player) return;

    // 削除対象のEnemyを一時保存
    std::vector<BossEnemy*> enemiesToDelete;

    for (BossEnemy* n_Enemy : m_enemyList)
    {
        // EnemyのCollisionObjectとPlayerのCharacterControllerで判定
        if (n_Enemy->collision && n_Enemy->collision->IsHit(player->characterController))
        {
            // ★ コインを50枚、範囲1000×1000で生成
            n_Enemy->SpawnCoins(n_Enemy->characterController.GetPosition(), 100, 1000.0f, 1000.0f);
            enemiesToDelete.push_back(n_Enemy);
        }
    }

    // 当たったEnemyを削除
    for (BossEnemy* n_Enemy : enemiesToDelete)
    {
        DeleteGO(n_Enemy); // GameObjectとして削除
        auto it = std::find(m_enemyList.begin(), m_enemyList.end(), n_Enemy);
        if (it != m_enemyList.end())
        {
            m_enemyList.erase(it);
        }
    }
}
/**敵マネージャーの初期設定メソッド*/
void EnemyManager::Setup()
{

}
/**新しい敵オブジェクトを生成して敵リストに追加するメソッド*/
void EnemyManager::CreateEnemy(Vector3& pos)
{
	BossEnemy* n_Enemy = NewGO<BossEnemy>(0,"enemy");/**(0,"hoge")0は処理の優先順位*/
	n_Enemy->SetPosition(pos);
	m_enemyList.push_back(n_Enemy);
}
/**敵を削除するメソッド*/
void EnemyManager::RemoveEnemy()
{
}
