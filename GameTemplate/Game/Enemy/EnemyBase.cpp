/**
 * @file EnemyBase.cpp
 * @brief 敵キャラクター基底クラスの実装
 */
#include "stdafx.h"
#include "EnemyBase.h"
#include "Game.h"
#include "Sound/SoundManager.h"


void EnemyBase::OnDead()
{
    // 撃破数を加算
    Game::DefeatedEnemyCount++;

    // 2. 死亡SEを再生 (Type.hに enSoundKind_DieEnemy がある前提)
    SoundManager::Get().PlaySE(enSoundKind_DieEnemy);
}