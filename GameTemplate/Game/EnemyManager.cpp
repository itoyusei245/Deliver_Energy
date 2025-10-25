/**“EnemyManager”敵キャラクターの管理を行うシングルトンパターンを用いたクラス*/
#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "StageManager.h"

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
}
/**敵マネージャーの初期設定メソッド*/
void EnemyManager::Setup()
{

}
/**新しい敵オブジェクトを生成して敵リストに追加するメソッド*/
void EnemyManager::CreateEnemy(Vector3& pos)
{
	Enemy* enemy = NewGO<Enemy>(0,"enemy");/**(0,"hoge")0は処理の優先順位*/
	enemy->SetPosition(pos);
	m_enemyList.push_back(enemy);
}
/**敵を削除するメソッド*/
void EnemyManager::RemoveEnemy()
{
}
