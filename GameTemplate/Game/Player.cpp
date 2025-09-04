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
    // 入力からXZ移動を作る
    moveSpeed.x = 0.0f;
    moveSpeed.z = 0.0f;

    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    Vector3 forward = g_camera3D->GetForward();
    Vector3 right = g_camera3D->GetRight();
    forward.y = 0.0f;
    right.y = 0.0f;

    right *= stickL.x * 180.0f;
    forward *= stickL.y * 180.0f;


    Vector3 inputMove = right + forward;

    if (isFallen) {
        // ================================
        // 倒れているとき → 転がり加速をつける
        // ================================
        if (inputMove.LengthSq() > 0.01f) {
            inputMove.Normalize();
            // 現在の速度に少しずつ加速
            rollVelocity += inputMove * 10.0f;   // 10.0f が加速の強さ
            // 最大スピード制限
            if (rollVelocity.Length() > 600.0f) {
                rollVelocity.Normalize();
                rollVelocity *= 600.0f;
            }
        }
        else {
            // 入力がなければ徐々に減速
            rollVelocity *= 0.5f;
        }

        moveSpeed += rollVelocity;
    }
    else {
        // 直立時 → 入力した分だけ移動
        moveSpeed += inputMove;
        // 倒立時は転がり速度をリセット
        rollVelocity = { 0,0,0 };
    }

    // ====== ジャンプ処理 ======
    if (characterController.IsOnGround()) {
        moveSpeed.y = 0.0f;
        if (g_pad[0]->IsTrigger(enButtonA)) {
            if (isFallen) {
                // 転がり速度があるとジャンプ強化
                float boost = rollVelocity.Length() * 0.4f;
                moveSpeed.y = 300.0f + boost;
            }
            else {
                moveSpeed.y = 400.0f;
            }
        }
    }
    else {
        moveSpeed.y -= 8.5f; // 重力
    }

    // 実際に移動
    position = characterController.Execute(moveSpeed, 1.0f / 60.0f);
    modelRender.SetPosition(position);
}


void Player::Rotation()
{
    // Bボタンで倒す／起き上がるを切り替え
    if (g_pad[0]->IsTrigger(enButtonB)) {
        if (!isFallen) {
            // 倒れる
            targetRotation.SetRotationDeg(Vector3::AxisX, 90.0f);
            isFallen = true;
            rollAngle = 0.0f;// 累積回転リセット
        }
        else {
            // 起き上がる
            targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
            isFallen = false;
			rollAngle = 0.0f;// 累積回転リセット
        }
    }

    // スティック入力方向を算出
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.z = g_pad[0]->GetLStickYF();
    Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

	if (inputDir.LengthSq() > 0.01f) {// 入力があるときのみ回転処理
		inputDir.Normalize();// 正規化

        if (isFallen) {
            // ================================
            // 倒れているとき：入力方向ベクトルを軸にゴロゴロ回転
            // ================================
			
			// 入力方向を向く回転
			Quaternion dirRot;

			// Y軸回転のみで向きを合わせる
			dirRot.SetRotationYFromDirectionXZ(inputDir);

			// 倒れたときの基本回転
			Quaternion fallenBase;
			
			// X軸90度倒れ
			fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

            // このフレームの移動距離
            float distance = moveSpeed.Length() * (1.0f / 60.0f);

            // 累積回転角度を更新（誇張するために倍率をかける）
            rollAngle += (distance / 25.0f) * 1.0f;

            // 入力方向をそのまま回転軸に利用
            Vector3 rollAxis = inputDir;
            rollAxis.Normalize();

			// 転がり回転
			Quaternion rollRot;

			// 軸と角度で回転をセット
            rollRot.SetRotation(rollAxis, rollAngle);

			// 最終的な目標回転を計算
            Quaternion tmp;
            tmp.Multiply(fallenBase, dirRot);
            targetRotation.Multiply(tmp, rollRot);
        }
        else {
            // ================================
            // 直立時：入力方向を前にする
            // ================================
            targetRotation.SetRotationYFromDirectionXZ(inputDir);
        }
    }

    // 回転補間
    rotation.Slerp(0.2f, rotation, targetRotation);

    // ====== 描画用の位置補正 ======
    Vector3 renderPos = position;

    if (!isFallen) {
        // 直立モード → pivot が中心なので、半径ぶん浮かせて底を接地
        renderPos.y += 40.0f;
    }
    else {
        // 倒れモード → pivot が中心なので、高さ半分ぶん浮かせて側面を接地
        renderPos.y += 25.0f;
    }

    // モデルに反映
    modelRender.SetPosition(renderPos);
    modelRender.SetRotation(rotation);
}



void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}