#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy() {
    modelRender.Init("Assets/animData/bossEnemy_TypeA.tkm");
    modelRender.SetPosition(100.0f, 0.0f, 0.0f);
    m_rot.SetRotationDegY(90);
    modelRender.SetRotation(m_rot);
    modelRender.SetScale(Vector3(1.2f, 1.2f, 1.2f));
    modelRender.Update();

    physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
    PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

    // 巡回ルートを設定（例: 四角形）
    waypoints.push_back(Vector3(100.0f, 0.0f, 0.0f));
    waypoints.push_back(Vector3(100.0f, 0.0f, 50.0f));
    waypoints.push_back(Vector3(50.0f, 0.0f, 50.0f));
    waypoints.push_back(Vector3(50.0f, 0.0f, 0.0f));
}

Enemy::~Enemy() {}

void Enemy::Update() {
    Move();
    Rotation();
    modelRender.Update();
}

void Enemy::Move() {
    if (waypoints.empty())return;

	Vector3 targetPos = waypoints[currentWaypoint];
    Vector3 dir = targetPos - currentPos;
    float distance = dir.Length();

    if(distance< 1.0f) { // 目標地点に到達したら次の地点へ
		currentWaypoint = (currentWaypoint + 1) % waypoints.size();
	}
    else
    {
		dir.Normalize();
		currentPos += dir * speed; // 速度を掛けて移動
        modelRender.SetPosition(currentPos);
    }
}

void Enemy::Rotation() {  
    Vector3 targetPos = waypoints[currentWaypoint];  
    Vector3 dir = targetPos - currentPos;  
    dir.y = 0; // 水平方向の回転のみ考慮  
    if (dir.Length() > 0.001f) {  
        dir.Normalize();  
        float angle = atan2f(dir.x, dir.z); // Z軸を基準に回転角を計算  
        m_rot.SetRotation(Vector3::AxisY, angle); // 修正: SetRotationに軸と角度を渡す  
        modelRender.SetRotation(m_rot);  
    }  
}

void Enemy::Render(RenderContext& rc) {
    modelRender.Draw(rc);
}
