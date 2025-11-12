#pragma once

/**
 * @brief 敵キャラクス
 * @details ゲーム内の敵オブジェクトを管理します。
 */
class NoobEnemy : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details 敵オブジェクトの初期化を行います。
     */
    NoobEnemy();

    /**
     * @brief デストラクタ
     * @details 敵オブジェクトの終了処理を行います。
     */
    ~NoobEnemy();

    bool Start();
    /**
     * @brief 更新処理
     * @details 毎フレーム呼ばれる敵の更新処理です。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @details 敵モデルの描画を行います。
     */
    void Render(RenderContext& rc);

    /**
     * @brief 移動処理
     * @details 敵の移動を制御します。
     */
    void Move();

    /**
     * @brief 回転処理
     * @details 敵の回転を制御します。
     */
    void Rotation();

    void SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ);

    void SetPosition(Vector3& pos) {
        currentPos = pos;
    }

    /** @brief モデルレンダラー */
    ModelRender m_noobEnemy;

    /** @brief キャラクターコントローラー（Playerと同じ） */
    CharacterController characterController;

    /** @brief 敵の回転（クォータニオン） */
    Quaternion m_rot;

    CollisionObject* collision;

private:
    /** @brief 巡回ポイントのリスト */
    std::vector<Vector3> waypoints;

    /** @brief 現在の巡回ポイントインデックス */
    int currentWaypoint = 0;

    /** @brief 移動速度 */
    float speed = 20.0f;

    /** @brief 現在位置 */
    Vector3 currentPos;
};
