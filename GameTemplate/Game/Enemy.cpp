#include "stdafx.h"
#include "Enemy.h"

/**
 * @brief Enemyクラスのコンストラクタ
 * @details モデル・キャラクターコントローラー・巡回ルートの初期化を行います。
 */
Enemy::Enemy() {
    modelRender.Init("Assets/animData/bossEnemy_TypeA.tkm");
    modelRender.SetScale(Vector3(1.2f, 1.2f, 1.2f));
    modelRender.Update();

    currentPos = { 100.0f, 0.0f, 0.0f };

    /** キャラコンを初期化（半径・高さ・初期位置）*/
    characterController.Init(25.0f, 50.0f, currentPos);

    /** 巡回ルートを設定*/
    waypoints.push_back(Vector3(100.0f, 0.0f, 0.0f));
    waypoints.push_back(Vector3(100.0f, 0.0f, 50.0f));
    waypoints.push_back(Vector3(50.0f, 0.0f, 50.0f));
    waypoints.push_back(Vector3(50.0f, 0.0f, 0.0f));
}

/**
 * @brief Enemyクラスのデストラクタ
 */
Enemy::~Enemy() {}

/**
 * @brief 毎フレームの更新処理
 * @details 移動・回転・モデルの更新を行います。
 */
void Enemy::Update() {
    Move();
    Rotation();
    modelRender.Update();
}

/**
 * @brief 敵の移動処理
 * @details 巡回ポイントに向かって移動し、到達したら次のポイントへ進みます。
 */
void Enemy::Move() {
    if (waypoints.empty()) return;

    Vector3 targetPos = waypoints[currentWaypoint];
    Vector3 dir = targetPos - currentPos;
    float distance = dir.Length();

    if (distance < 1.0f) {
        /** 巡回ポイント到達時、次のポイントへ*/
        currentWaypoint = (currentWaypoint + 1) % waypoints.size();
    }
    else {
        dir.Normalize();
        Vector3 move = dir * speed;

        /** キャラコンを使って移動（当たり判定つき*/
        currentPos = characterController.Execute(move, 1.0f / 60.0f);

        /** モデルに反映*/
        modelRender.SetPosition(currentPos);
    }
}

/**
 * @brief 敵の回転処理
 * @details 移動方向にモデルの向きを合わせます。
 */
void Enemy::Rotation() {
    Vector3 targetPos = waypoints[currentWaypoint];
    Vector3 dir = targetPos - currentPos;
    dir.y = 0;

    if (dir.Length() > 0.001f) {
        dir.Normalize();
        float angle = atan2f(dir.x, dir.z);
        m_rot.SetRotation(Vector3::AxisY, angle);
        modelRender.SetRotation(m_rot);
    }
}

/**
 * @brief 敵の描画処理
 * @param rc 描画コンテキスト
 * @details モデルの描画を行います。
 */
void Enemy::Render(RenderContext& rc) {
    modelRender.Draw(rc);
}
