/**
 * @file Player.cpp
 * @brief プレイヤーの実装
 */
#include "stdafx.h"
#include "Player.h"
#include"Game.h"

 /**
  * @brief コンストラクタ
  * @details 缶モデルのロード、初期位置(-200.0f)、CharacterControllerの初期化を行います。
  */
Player::Player()
{
    /**缶のモデルを読み込む*/
    modelRender.Init("Assets/animData/Player.tkm");
    position = { 0.0f, -200.0f, 0.0f };

    /**キャラクターコントローラーを初期化（半径25, 高さ50）*/
    characterController.Init(25.0f, 50.0f, position);

    rotation.SetRotationDeg(Vector3::AxisX, 0.0f);
    targetRotation = rotation;
}

Player::~Player()
{
}

void Player::Update()
{
    // ポーズ中は停止
    if (Game::IsPaused) return;

    /**移動処理*/
    Move();

    /**回転処理*/
    Rotation();

    // ========== テスト用コード ==========

    // HPを少しずつ減らす (60FPSなら1秒で約30減る)
    m_hp -= 0.1f;

    // 0以下になったら満タンに戻してループさせる
    if (m_hp <= 0.0f) {
        m_hp = m_maxHp;
    }
    /**モデルの更新処理*/
    modelRender.Update();
}

/**
 * @brief 移動処理
 * @details
 * - カメラの向きを基準に左スティック入力で移動ベクトルを作成します。
 * - **isFallen（倒れ状態）** の場合：
 * - 入力方向に徐々に加速し、慣性が働く「転がり移動」を行います。
 * - 入力がなければ徐々に減速します。
 * - **直立状態** の場合：
 * - 入力に対してダイレクトに移動します。
 * - ジャンプ(Aボタン)：
 * - 倒れているときは速度に応じてジャンプ力が変化します。
 */
void Player::Move()
{
    /**入力からXZ移動を作成*/
    moveSpeed.x = 0.0f;
    moveSpeed.z = 0.0f;

    // カメラ基準の入力変換
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

    // --- 状態による移動ロジック分岐 ---
    if (isFallen) {
        /** 倒れ時：慣性のある転がり移動 */
        if (inputMove.LengthSq() > 0.01f) {
            inputMove.Normalize();
            /** 現在の速度に少しずつ加速（10.0fが加速係数）*/
            rollVelocity += inputMove * 10.0f;
            /** 最大スピード制限 (600.0f) */
            if (rollVelocity.Length() > 600.0f) {
                rollVelocity.Normalize();
                rollVelocity *= 600.0f;
            }
        }
        else {
            /** 入力がなければ摩擦で減速 */
            rollVelocity *= 0.5f;
        }
        moveSpeed += rollVelocity;
    }
    else {
        /** 直立時：直接移動 */
        moveSpeed += inputMove;
        rollVelocity = { 0,0,0 };
    }

    // --- ジャンプと重力 ---
    if (characterController.IsOnGround()) {
        moveSpeed.y = 0.0f;
        if (g_pad[0]->IsTrigger(enButtonA)) {
            if (isFallen) {
                /** 転がり中は速度に応じてジャンプ力アップ */
                float boost = rollVelocity.Length() * 0.4f;
                moveSpeed.y = 300.0f + boost;
            }
            else {
                moveSpeed.y = 400.0f;
            }
        }
    }
    else {
        /** 重力適用 */
        moveSpeed.y -= 8.5f;
    }

    /** CharacterControllerで移動を実行し、結果の座標を取得 */
    position = characterController.Execute(moveSpeed, 1.0f / 60.0f);
    modelRender.SetPosition(position);
}

/**
 * @brief 回転処理
 * @details
 * - Bボタンで直立/倒れ状態を切り替えます。
 * - 移動量に応じてモデルを回転させ、転がっているように見せます。
 * - 直立時と倒れ時でモデルの中心位置（Y座標）を補正し、地面に接するようにします。
 */
void Player::Rotation()
{
    /** Bボタンで倒す／起き上がるを切り替え*/
    if (g_pad[0]->IsTrigger(enButtonB)) {
        if (!isFallen) {
            /** 倒れる（X軸90度）*/
            targetRotation.SetRotationDeg(Vector3::AxisX, 90.0f);
            isFallen = true;
            rollAngle = 0.0f;
        }
        else {
            /** 起き上がる（X軸0度）*/
            targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
            isFallen = false;
            rollAngle = 0.0f;
        }
    }

    /** 入力方向へ向く処理 */
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.z = g_pad[0]->GetLStickYF();
    Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

    if (inputDir.LengthSq() > 0.01f) {
        inputDir.Normalize();

        if (isFallen) {
            /**
             * 倒れているとき：進行方向を向いた上で、移動距離分だけゴロゴロ自転させる
             */

             // 1. 進行方向への向き
            Quaternion dirRot;
            dirRot.SetRotationYFromDirectionXZ(inputDir);

            // 2. 倒れた基本姿勢
            Quaternion fallenBase;
            fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

            // 3. 移動距離に応じた自転角度の更新
            float distance = moveSpeed.Length() * (1.0f / -60.0f);
            rollAngle += (distance / 25.0f) * 1.0f; // 25.0fは半径相当の係数

            // 4. 入力方向を軸とした回転
            Vector3 rollAxis = inputDir;
            rollAxis.Normalize();
            Quaternion rollRot;
            rollRot.SetRotation(rollAxis, rollAngle);


            Vector3 cameraDirection = g_camera3D->GetPosition() - g_camera3D->GetTarget();
            cameraDirection.Normalize();

            Quaternion cameraRot;

            cameraRot.SetRotationYFromDirectionXZ(cameraDirection);

            cameraRot.AddRotationDegY(90.0f*2);


            // 合成：基本倒れ * 向き * 自転
            Quaternion tmp;
            tmp.Multiply(fallenBase, dirRot);
            targetRotation.Multiply(tmp, rollRot);
            targetRotation.Multiply(cameraRot);
        }
        else {
            /** 直立時：シンプルに入力方向を向く */
            targetRotation.SetRotationYFromDirectionXZ(inputDir);
        }
    }

    /** 回転を滑らかに補間 */
    rotation.Slerp(0.2f, rotation, targetRotation);

    /** 描画位置のY座標補正（ピボットが中心にあるため） */
    Vector3 renderPos = position;
    if (!isFallen) {
        renderPos.y += 40.0f; // 直立時の底合わせ
    }
    else {
        renderPos.y += 25.0f; // 倒れ時の側面合わせ
    }

    modelRender.SetPosition(renderPos);
    modelRender.SetRotation(rotation);
}

void Player::Render(RenderContext& rc)
{
    modelRender.Draw(rc);
}