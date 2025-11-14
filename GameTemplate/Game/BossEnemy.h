#pragma once

/**
 * @brief BossEnemyクラス
 * @details 上下に移動するボス敵の管理クラス。
 *          本体モデル（m_bossEnemy）と動く土台モデル（m_body）を持ち、
 *          当たり判定・コイン生成・撃破演出などを制御します。
 */
class BossEnemy : public IGameObject
{
public:
    BossEnemy();
    ~BossEnemy();

    /** 初期化 */
    bool Start();

    /** 毎フレーム更新 */
    void Update();

    /** 描画処理 */
    void Render(RenderContext& rc);

    /** コインを生成（EnemyManagerが呼び出す） */
    void SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ);

    /** ボス撃破時に呼ばれる（EnemyManagerが呼び出す） */
    void OnDefeated();

    /** 外部から初期位置を指定する場合 */
    void SetPosition(Vector3& pos)
    {
        m_bossPos = pos;
        m_bodyPos = pos - Vector3(0.0f, 120.0f, 0.0f);
    }

public:
    /** キャラクターコントローラー（位置管理用） */
    CharacterController characterController;

    /** 当たり判定 */
    CollisionObject* collision = nullptr;

private:
    /** 本体モデル（ボス） */
    ModelRender m_bossEnemy;

    /** 動く土台モデル */
    ModelRender m_body;

    /** ボスの位置 */
    Vector3 m_bossPos = Vector3::Zero;

    /** 土台の位置 */
    Vector3 m_bodyPos = Vector3::Zero;

    /** 回転情報 */
    Quaternion m_rot = Quaternion::Identity;

    /** 上下移動に関するパラメータ */
    bool m_movingUp = true;
    float m_moveSpeed = 100.0f;
    float m_minY = -120.0f;
    float m_maxY = 80.0f;

    /** 生存状態（撃破後はfalse） */
    bool m_isAlive = true;

private:
    /** 上下移動処理 */
    void Move();

    /** 回転処理 */
    void Rotation();
};
