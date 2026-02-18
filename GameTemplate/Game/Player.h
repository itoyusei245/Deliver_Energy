/**
 * @file Player.h
 * @brief プレイヤーキャラクター（缶）のクラス定義
 */
#pragma once
#include "Sound/SoundManager.h"
 /**
  * @class Player
  * @brief プレイヤー操作キャラクタークラス
  * @details CharacterControllerを使用した移動、ジャンプに加え、
  * 「倒れて転がる（Roll）」という独自の挙動を持っています。
  */
class Player : public IGameObject
{
public:
    Player();
    ~Player();

    void Update();
    void Render(RenderContext& rc);

    const Vector3& GetPosition() const {
        return position;
    }

    /**
     * @brief 移動制御
     * @details 左スティック入力による移動、ジャンプ、および転がり時の加速処理を行います。
     */
    void Move();

    /**
     * @brief 回転制御
     * @details 入力方向への旋回や、倒れた状態でのゴロゴロ回転（自転）を計算します。
     */
    void Rotation();

    /** @name コンポーネント */
    ModelRender modelRender;
    CharacterController characterController;
    

    /** 現在の座標 */
    Vector3 position;

    /** @name 移動パラメータ */
    
    Vector3 moveSpeed;              /**< 現在の移動速度ベクトル */
    Vector3 rollVelocity = { 0.0f, 0.0f, 0.0f }; /**< 転がり時の慣性速度 */
    int jump = 0;                   /**< ジャンプ状態（※現在Move内で直接処理しているため未使用の可能性あり） */
    

    /** @name 回転制御用 */
    Quaternion targetRotation;      /**< 目標とする回転 */
    Quaternion rotation;            /**< 現在の回転 */
    bool isFallen = false;          /**< 倒れている（転がりモード）かどうか */
    float rollAngle = 0.0f;         /**< 転がり回転の累積角度 */
    // HPの割合を取得する関数s
    float GetHPRate() const {
        return m_hp / m_maxHp;
    }

    // ダメージ処理などのテスト用
    void Damage(float damage) {
        m_hp -= damage;
        if (m_hp < 0.0f) m_hp = 0.0f;
    }

    // プレイヤーの動きを止めるための関数(デバッグ時など)
    void SetEnable(bool isEnable) { m_isEnable = isEnable; }

private:

    float m_hp = 100.0f;
    float m_maxHp = 100.0f;
    SoundHandle m_walkHandle = INVALID_SOUND_HANDLE;    // 歩き音のハンドル
    SoundHandle m_rollHandle = INVALID_SOUND_HANDLE;    // 回転音のハンドル
    bool m_isGroundPrev = true;                         // 1フレーム前の接地状態（着地判定用）
    bool m_isEnable = true;                             // 有効フラグ (初期値は true)
};