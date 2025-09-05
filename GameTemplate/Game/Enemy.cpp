#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//modelRender.Init("Assets/animData/noobEnemy.tkm");
	modelRender.Init("Assets/animData/bossEnemy_TypeA.tkm");
	characterController.Init(25.0f, 50.0f, position);
	position = { 100.0f, 100.0f, 100.0f };

	
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	
}

void Enemy::Move()
{
	
}

void Enemy::Rotation()
{
	
}

void Enemy::Render(RenderContext&rc)
{
	modelRender.Draw(rc);
}
