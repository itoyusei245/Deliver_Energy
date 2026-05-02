/**
 * @file EnemyStatus.cpp
 * @brief 敵ステータスの実装
 */
#include "stdafx.h"
#include "EnemyStatus.h"

namespace 
{
	constexpr int   BOSS_MAX_HP		= 3;
	constexpr float BOSS_MOVE_SPEED	= 100.0f;

	constexpr int   FAMILIAR_MAX_HP		= 1;
	constexpr float FAMILIAR_MOVE_SPEED	= 100.0f;

	constexpr int   NOOB_MAX_HP		= 1;
	constexpr float NOOB_MOVE_SPEED	= 100.0f;
}

void BossStatus::Setup()
{
	m_maxHP		= BOSS_MAX_HP;
	m_currentHP = m_maxHP;
	m_moveSpeed = BOSS_MOVE_SPEED;
}

void FamiliarStatus::Setup()
{
	m_maxHP		= FAMILIAR_MAX_HP;
	m_currentHP = m_maxHP;
	m_moveSpeed = FAMILIAR_MOVE_SPEED;
}

void NoobEnemyStatus::Setup()
{
	m_maxHP		= NOOB_MAX_HP;
	m_currentHP = m_maxHP;
	m_moveSpeed = NOOB_MOVE_SPEED;
}