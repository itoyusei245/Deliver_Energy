#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(450.0f, 0.0f,0.0f);
	//プレイヤーのインスタンスを探す。
	player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	//カメラを更新。
	//注視点を計算する。
	Vector3 target = player->position;
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 60.0f;

	Vector3 toCameraPosOld = m_toCameraPos;

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;
	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新。
	g_camera3D->Update();
}