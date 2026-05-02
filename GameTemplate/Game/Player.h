/**
 * @file Player.h
 * @brief プレイヤーキャラクター（缶）のクラス定義
 */
#pragma once
#include "Sound/SoundManager.h"

class Player : public IGameObject
{
public:
    Player();
    ~Player();

    void Update() override;
    void Render(RenderContext& rc) override;

    const Vector3& GetPosition() const {
        return position;
    }

    void Move();
    void Rotation();

    // コンポーネント
    ModelRender modelRender;
    CharacterController characterController;

    // 座標・移動パラメータ
    Vector3 position;
    Vector3 moveSpeed;
    Vector3 rollVelocity = { 0.0f, 0.0f, 0.0f };

    // 回転制御用
    Quaternion targetRotation;
    Quaternion rotation;
    bool isFallen = false;
    float rollAngle = 0.0f;

    // HPの割合を取得する関数
    float GetHPRate() const {
        return m_hp / m_maxHp;
    }

    // ダメージ処理などのテスト用
    void Damage(float damage) {
        m_hp -= damage;
        if (m_hp < 0.0f) m_hp = 0.0f;
    }

    // プレイヤーの動きを止めるための関数
    void SetEnable(bool isEnable) { m_isEnable = isEnable; }

private:
    float m_hp = 100.0f;
    float m_maxHp = 100.0f;

    SoundHandle m_walkHandle = INVALID_SOUND_HANDLE;
    SoundHandle m_rollHandle = INVALID_SOUND_HANDLE;
    bool m_isGroundPrev = true;
    bool m_isEnable = true;
};