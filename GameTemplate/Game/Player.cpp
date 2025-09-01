#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//ユニティちゃんのモデルを読み込む
	modelRender.Init("Assets/animData/Player.tkm");

	//キャラコンを初期化
	characterController.Init(10.0f, 10.0f, position);
	position = { 10.0f, 10.0f, 0.0f };//初期位置

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

	//m_sound->SetVolume(3.5f);

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
		//m_sound->Stop();
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
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		rotation.SetRotationYFromDirectionXZ(moveSpeed);

		//絵描きさんに回転を教える。
		modelRender.SetRotation(rotation);

	}

}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}