#include "stdafx.h"
#include "DebugCamera.h"
#include <stdio.h>
#include <windows.h> // 出力ウィンドウ用

DebugCamera::DebugCamera()
{
}

DebugCamera::~DebugCamera()
{
}

bool DebugCamera::Start()
{
	return true;
}

void DebugCamera::Update()
{
	Move();
	Rotation();


	// ---------------------------------------------------
	// 座標出力
	// ---------------------------------------------------
	if (g_pad[0]->IsTrigger(enButtonA)) {
		Camera* cam = g_camera3D;
		Vector3 pos = cam->GetPosition();
		Vector3 target = cam->GetTarget();


		char buffer[256];

		sprintf_s(buffer, "m_pathList.push_back({ Vector3(%ff, %ff, %ff), Vector3(%ff, %ff, %ff) });\n",
			pos.x, pos.y, pos.z, target.x, target.y, target.z);

		// Visual Studioの「出力」タブに表示
		OutputDebugStringA(buffer);
	}
}


void DebugCamera::Move()
{
	//カメラ情報の取得
	Camera* cam = g_camera3D;
	Vector3 pos = cam->GetPosition();
	Vector3 target = cam->GetTarget();


	//カメラ方向の計算
	Vector3 forward = target - pos;
	forward.Normalize();


	Vector3 right;
	right.Cross(Vector3::AxisY, forward);
	right.Normalize();


	Vector3 up = Vector3::AxisY;


	// 移動スピード
	float speed = 300.0f * g_gameTime->GetFrameDeltaTime();
	if (g_pad[0]->IsPress(enButtonRB2)) {
		speed *= 4.0f;
	}


	Vector3 move = Vector3::Zero;
	//前後左右
	move += forward * g_pad[0]->GetLStickYF();
	move += right * g_pad[0]->GetLStickXF();


	if (g_pad[0]->IsPress(enButtonRB1))move += up;
	if (g_pad[0]->IsPress(enButtonLB1))move -= up;


	//計算した移動量を座標と注視点に加算
	pos += move * speed;
	target += move * speed;


	//カメラに反映
	cam->SetPosition(pos);
	cam->SetTarget(target);
	cam->Update();
}


void DebugCamera::Rotation()
{
	//カメラ情報の取得
	Camera* cam = g_camera3D;
	Vector3 pos = cam->GetPosition();
	Vector3 target = cam->GetTarget();


	//回転スピード
	float rotSpeed = 2.0f * g_gameTime->GetFrameDeltaTime();
	

	float rx = g_pad[0]->GetRStickXF() * rotSpeed;	//横
	float ry = g_pad[0]->GetRStickYF() * rotSpeed;	//縦


	//注視点を中心ではなく、カメラ位置を中心に回転させる
	Vector3 toTarget = target - pos;


	//横回転
	Quaternion qy;
	qy.SetRotationDeg(Vector3::AxisY, rx * 100.0f);
	qy.Apply(toTarget);


	//縦回転
	Vector3 forward = toTarget;
	forward.Normalize();
	Vector3 right;
	right.Cross(Vector3::AxisY, forward);
	right.Normalize();


	Quaternion qx;
	qx.SetRotationDeg(right, -ry * 100.0f);
	qx.Apply(toTarget);


	// 新しい注視点を設定
	target = pos + toTarget;


	// カメラに反映
	cam->SetPosition(pos);
	cam->SetTarget(target);
	cam->Update();
}