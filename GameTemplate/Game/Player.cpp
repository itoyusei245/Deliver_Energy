#include "stdafx.h"
#include "Player.h"
#include"sound/SoundEngine.h"

Player::Player()
{
	//ユニティちゃんのモデルを読み込む
	modelRender.Init("Assets/modelData/Player.tkm", animationClips,enAnimationClip_Num, enModelUpAxisY);
	//modelRender.Update();

	//キャラコンを初期化
	characterController.Init(10.0f, 10.0f, position);
	position = { -20.0f, -600.0f, 0.0f };//初期位置

}

Player::~Player()
{
	DeleteGO(m_sound);
}

//更新処理
void Player::Update()
{
	//移動処理。
	Move();

	//回転処理。
	Rotation();

	//ステート管理。
	ManageState();

	//アニメーションの再生。
	Animation();

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
			g_soundEngine->ResistWaveFileBank(0, "Assets/Sound/jump.wav");
			m_sound = NewGO<SoundSource>(0);
			m_sound->Init(0);
			m_sound->Play(false);

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
	else if (jump == 0) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			moveSpeed.y += 300.0f;
			jump = 1;
			playerState = 1;
		}
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

//ステート管理
void Player::ManageState()
{
	//地面に付いていなかったら。
	if (characterController.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}

	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//ステートを2(歩き)にする。
		playerState = 2;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする。
		playerState = 0;
	}
}

void Player::Animation()
{
	//switch文。
	switch (playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}