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

    SoundManager::Get().PlaySE(enSoundKind_DieEnemy);
}