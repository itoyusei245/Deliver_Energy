/**
 * @file Player.cpp
 * @brief プレイヤーの実装
 */
#include "stdafx.h"
#include "Player.h"
#include "Camera/EventCamera.h"
#include "Game.h"
#include "Sound/SoundManager.h"

namespace {
    constexpr const char* PATH_PLAYER_MODEL = "Assets/animData/Player.tkm";

    constexpr float CHARA_RADIUS = 25.0f;
    constexpr float CHARA_HEIGHT = 50.0f;
    const Vector3 POS_START(0.0f, -200.0f, 0.0f);
    const Vector3 POS_ZERO(0.0f, 0.0f, 0.0f);

    constexpr float INPUT_DEADZONE = 0.01f;  
    constexpr float MOVE_SPEED_BASE = 180.0f;
    constexpr float ROLL_ACCEL = 10.0f;  
    constexpr float ROLL_MAX_SPEED = 600.0f; 
    constexpr float ROLL_FRICTION = 0.5f;   

    constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f; 

    constexpr float JUMP_FORCE_NORMAL = 400.0f; 
    constexpr float JUMP_FORCE_ROLL_BASE = 300.0f; 
    constexpr float JUMP_ROLL_BOOST_RATE = 0.4f;   
    constexpr float GRAVITY_FORCE = 8.5f;   

    constexpr float FALLEN_ANGLE = 90.0f; 
    constexpr float ROTATION_SLERP_SPEED = 0.2f;  
    constexpr float RENDER_OFFSET_Y_NORMAL = 40.0f; 
    constexpr float RENDER_OFFSET_Y_FALLEN = 25.0f; 
}

Player::Player()
{
    modelRender.Init(PATH_PLAYER_MODEL);
    position = POS_START;

    characterController.Init(CHARA_RADIUS, CHARA_HEIGHT, position);

    rotation.SetRotationDeg(Vector3::AxisX, 0.0f);
    targetRotation = rotation;
}

Player::~Player()
{
}

void Player::Update()
{
    if (EventCamera::IsEventPlaying) {
        return;
    }
    if (!m_isEnable) return;
    if (Game::IsPaused) return;

    Move();
    Rotation();

    modelRender.Update();
}

void Player::Move()
{
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

    right *= stickL.x * MOVE_SPEED_BASE;
    forward *= stickL.y * MOVE_SPEED_BASE;

    Vector3 inputMove = right + forward;

    // 現在の接地判定を取得
    bool isGrounded = characterController.IsOnGround();

    // ==========================================
    // サウンド再生処理
    // ==========================================
    bool isWalkng = isGrounded && !isFallen && (inputMove.LengthSq() > INPUT_DEADZONE);

    if (isWalkng) {
        if (m_walkHandle == INVALID_SOUND_HANDLE) {
            m_walkHandle = SoundManager::Get().PlaySE(enSoundKind_Walk, true);
        }
    }
    else {
        if (m_walkHandle != INVALID_SOUND_HANDLE) {
            SoundManager::Get().StopSE(m_walkHandle);
            m_walkHandle = INVALID_SOUND_HANDLE;
        }
    }

    bool isRolling = isGrounded && isFallen && (rollVelocity.Length() > 10.0f);

    if (isRolling) {
        if (m_rollHandle == INVALID_SOUND_HANDLE) {
            m_rollHandle = SoundManager::Get().PlaySE(enSoundKind_Walk, true);
        }
    }
    else {
        if (m_rollHandle != INVALID_SOUND_HANDLE) {
            SoundManager::Get().StopSE(m_rollHandle);
            m_rollHandle = INVALID_SOUND_HANDLE;
        }
    }

    // ==========================================
    // 移動ロジック分岐
    // ==========================================
    if (isFallen) {
        // 倒れ時：慣性のある転がり移動
        if (inputMove.LengthSq() > INPUT_DEADZONE) {
            inputMove.Normalize();
            rollVelocity += inputMove * ROLL_ACCEL;

            if (rollVelocity.Length() > ROLL_MAX_SPEED) {
                rollVelocity.Normalize();
                rollVelocity *= ROLL_MAX_SPEED;
            }
        }
        else {
            // 入力がなければ摩擦で減速
            rollVelocity *= ROLL_FRICTION;
        }
        moveSpeed += rollVelocity;
    }
    else {
        // 直立時：直接移動
        moveSpeed += inputMove;
        rollVelocity = POS_ZERO;
    }

    // ==========================================
    // ジャンプと重力
    // ==========================================
    if (characterController.IsOnGround()) {
        moveSpeed.y = 0.0f;

        if (g_pad[0]->IsTrigger(enButtonA)) {
            SoundManager::Get().PlaySE(enSoundKind_Jump);
            if (isFallen) {
                float boost = rollVelocity.Length() * JUMP_ROLL_BOOST_RATE;
                moveSpeed.y = JUMP_FORCE_ROLL_BASE + boost;
            }
            else {
                moveSpeed.y = JUMP_FORCE_NORMAL;
            }
        }
    }
    else {
        moveSpeed.y -= GRAVITY_FORCE;
    }

    position = characterController.Execute(moveSpeed, FIXED_DELTA_TIME);
    modelRender.SetPosition(position);
}

void Player::Rotation()
{
    // Bボタンで倒す／起き上がるを切り替え
    if (g_pad[0]->IsTrigger(enButtonB)) {
        if (!isFallen) {
            targetRotation.SetRotationDeg(Vector3::AxisX, FALLEN_ANGLE);
            isFallen = true;
            rollAngle = 0.0f;
        }
        else {
            targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
            isFallen = false;
            rollAngle = 0.0f;
        }
    }

    // 入力方向へ向く処理
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.z = g_pad[0]->GetLStickYF();
    Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

    if (inputDir.LengthSq() > INPUT_DEADZONE) {
        inputDir.Normalize();

        if (isFallen) {
            // 1. 進行方向への向き
            Quaternion dirRot;
            dirRot.SetRotationYFromDirectionXZ(inputDir);

            // 2. 倒れた基本姿勢
            Quaternion fallenBase;
            fallenBase.SetRotationDeg(Vector3::AxisX, FALLEN_ANGLE);

            // 3. 移動距離に応じた自転角度の更新
            float distance = moveSpeed.Length() * -FIXED_DELTA_TIME;
            rollAngle += (distance / CHARA_RADIUS) * 1.0f;

            // 4. 入力方向を軸とした回転
            Vector3 rollAxis = inputDir;
            rollAxis.Normalize();
            Quaternion rollRot;
            rollRot.SetRotation(rollAxis, rollAngle);

            Vector3 cameraDirection = g_camera3D->GetPosition() - g_camera3D->GetTarget();
            cameraDirection.Normalize();

            Quaternion cameraRot;
            cameraRot.SetRotationYFromDirectionXZ(cameraDirection);
            cameraRot.AddRotationDegY(180.0f); 

            // 合成：基本倒れ * 向き * 自転
            Quaternion tmp;
            tmp.Multiply(fallenBase, dirRot);
            targetRotation.Multiply(tmp, rollRot);
            targetRotation.Multiply(cameraRot);
        }
        else {
            targetRotation.SetRotationYFromDirectionXZ(inputDir);
        }
    }

    rotation.Slerp(ROTATION_SLERP_SPEED, rotation, targetRotation);

    // 描画位置のY座標補正
    Vector3 renderPos = position;
    if (!isFallen) {
        renderPos.y += RENDER_OFFSET_Y_NORMAL;
    }
    else {
        renderPos.y += RENDER_OFFSET_Y_FALLEN;
    }

    modelRender.SetPosition(renderPos);
    modelRender.SetRotation(rotation);
}

void Player::Render(RenderContext& rc)
{
    modelRender.Draw(rc);
}