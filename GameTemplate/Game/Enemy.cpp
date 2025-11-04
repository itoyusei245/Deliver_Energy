#include "stdafx.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "GetItem.h"
#include <random>
#include <ctime>
#include <cstdlib>
/**
 * @brief Enemyクラスのコンストラクタ
 * @details モデル・キャラクターコントローラー・巡回ルートの初期化を行います。
 */
Enemy::Enemy() {

}

/**
 * @brief Enemyクラスのデストラクタ
 */
Enemy::~Enemy() {}

bool Enemy::Start()
{
    modelRender.Init("Assets/animData/main_bossEnemy.tkm");
    modelRender.SetScale(Vector3(80.0f, 80.0f, 80.0f));
    modelRender.Update();

    // キャラコンのパラメータ
    float radius = 25.0f;
    float height = 50.0f;
    characterController.Init(radius, height, currentPos);

    // CollisionObject生成
    collision = NewGO<CollisionObject>(0, "CollisionObject");

    // キャラコンと同じサイズでBoxを作成
    Vector3 boxSize(radius, height, radius);

    collision->CreateBox(currentPos, m_rot, boxSize);

    collision->SetIsEnableAutoDelete(false);   //ここでfalseを設定しないとキャラコンが次のフレームで勝手に消される

    // 巡回ルートを設定
    waypoints.push_back(Vector3(3500.0f, -100.0f, -4200.0f));
    waypoints.push_back(Vector3(3500.0f, -100.0f, -4100.0f));
    waypoints.push_back(Vector3(3400.0f, -100.0f, -4100.0f));
    waypoints.push_back(Vector3(3400.0f, -100.0f, -4200.0f));
    return true;
}
/**
 * @brief 毎フレームの更新処理
 * @details 移動・回転・モデルの更新を行います。
 */
void Enemy::Update() {
    Move();
    Rotation();
    modelRender.Update();

    Player* player = FindGO<Player>("player");
    if (!player)
    {
        return;
    }

    collision->SetPosition(currentPos);
    collision->SetRotation(m_rot);
    if (collision->IsHit(player->characterController)) {
        EnemyManager::GetInstance()->RemoveEnemy();
    }
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

void Enemy::SpawnCoins(const Vector3& center, int count, float rangeX, float rangeZ)
{
    srand((unsigned int)time(nullptr));
    for (int i = 0; i < count; ++i)
    {
        /**-rangeX~+rangeXの範囲で乱数を出す*/
        float offsetX = ((float)rand() / RAND_MAX) * 2.0f * rangeX - rangeZ;
        float offsetZ = ((float)rand() / RAND_MAX) * 2.0f * rangeZ - rangeZ;

        /**出す位置を計算*/
        Vector3 pos = center;
        pos.x += offsetX;
        pos.z += offsetZ;
        pos.y += 10.0f;

        /**コイン生成*/
        GetItem* coin = NewGO<GetItem>(0, "coin");
        coin->Init(pos);
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
