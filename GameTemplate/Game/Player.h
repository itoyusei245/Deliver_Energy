#pragma once

/**
 * @brief プレイヤークラス
 * @details ゲーム内のプレイヤーオブジェクトを管理します。
 */
class Player : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details NewGOで作成されたときに呼ばれます。
     */
    Player();

    /**
     * @brief デストラクタ
     * @details DeleteGOで削除されたときに呼ばれます。
     */
    ~Player();

    /**
     * @brief 更新処理
     * @details 毎フレーム呼ばれる更新処理です。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @details プレイヤーの描画を行います。
     */
    void Render(RenderContext& rc);

    /**
     * @brief 移動処理
     * @details プレイヤーの移動を制御します。
     */
    void Move();

    /**
     * @brief 回転処理
     * @details プレイヤーの回転を制御します。
     */
    void Rotation();

    /** @name メンバ変数*/

    /** @brief モデルレンダラー */
    ModelRender modelRender;

    /** @brief 座標 */
    Vector3 position;

    /** @brief 倒れているときの転がり速度 */
    Vector3 rollVelocity = { 0.0f, 0.0f, 0.0f };

    /** @brief 移動速度 */
    Vector3 moveSpeed;

    /** @brief キャラクターコントローラー */
    CharacterController characterController;

    /** @brief 目標の回転 */
    Quaternion targetRotation;

    /** @brief 現在の回転（クォータニオン） */
    Quaternion rotation;

    /** @brief ジャンプ状態 */
    int jump = 0;

    /** @brief 倒れているかどうか */
    bool isFallen = false;

    /** @brief ゴロゴロ用の回転角 */
    float rollAngle = 0.0f;

};

