#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	//ユニティちゃんのモデルを読み込む
	modelRender.Init("Assets/animData/Player.tkm");

	//キャラコンを初期化
	characterController.Init(25.0f, 50.0f, position);
	position = { 10.0f, 10.0f, 0.0f };//初期位置
	rotation.SetRotationDeg(Vector3::AxisX, 0.0f);
	targetRotation = rotation;

}

Player::~Player()
{
}

//更新処理
void Player::Update()
{
	//移動処理。
	Move();

	
	

	//回転処理。
	Rotation();

	//絵描きさんの更新処理。
	modelRender.Update();

}

void Player::Move()
{
	//xzの移動速度を0.0fにする。
	moveSpeed.x =0.0f;
	moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 125.0f;
	forward *= stickL.y * 125.0f;

	//移動速度にスティックの入力量を加算する。
	moveSpeed += right + forward;
	if (jumpState == 0)
	{

	}
	//地面に付いていたら。
	if (characterController.IsOnGround())
	{
		//重力を無くす。
		moveSpeed.y = 0.0f;

		//Aボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ジャンプさせる。
			moveSpeed.y = 300.0f;
			jump += 1;

		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		moveSpeed.y -= 7.0f;
	}

	if (characterController.IsOnGround() == true) {
		playerState = 0;

		jump = 0;
	}
	//キャラクターコントローラーを使って座標を移動させる。
	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	modelRender.SetPosition(position);
}

void Player::Rotation()
{
	// Bボタンで倒す／起き上がるを切り替え
	if (g_pad[0]->IsTrigger(enButtonB)) {
		if (!isFallen) {
			// 倒れる姿勢を目標に
			targetRotation.SetRotationDeg(Vector3::AxisX, 90.0f);
			isFallen = true;
		}
		else {
			// 立ち姿勢を目標に
			targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
			isFallen = false;
		}
	}

	// スティック入力方向を算出
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();
	Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

	if (inputDir.LengthSq() > 0.01f) {
		inputDir.Normalize();

		if (isFallen) {
			// ================================
			// 倒れているとき：横倒し + 入力方向を前にする
			// ================================
			Quaternion dirRot;
			dirRot.SetRotationYFromDirectionXZ(inputDir);

			Quaternion fallenBase;
			fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

			targetRotation.Multiply(fallenBase, dirRot);
		}
		else {
			// ================================
			// 直立時：入力方向を前にする
			// ================================
			targetRotation.SetRotationYFromDirectionXZ(inputDir);
		}
	}

	// 補間して回転
	rotation.Slerp(0.2f, rotation, targetRotation);

	// モデルに反映
	modelRender.SetRotation(rotation);
}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}