#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include"Game.h"

/**
 * @brief GameCameraクラスのコンストラクタ
 */
GameCamera::GameCamera()
{
	
}

/**
 * @brief GameCameraクラスのデストラクタ
 */
GameCamera::~GameCamera()
{

}

/**
 * @brief カメラの初期化処理
 * @details 注視点から視点までのベクトル設定、プレイヤーの取得、カメラのニア・ファークリップ設定を行います。
 * @return 初期化が成功した場合はtrue
 */
bool GameCamera::Start()
{
    /** 注視点から視点までのベクトルを設定*/
    m_toCameraPos.Set(-450.0f, 0.0f, 0.0f);

    if (player == nullptr) {
        player = FindGO<Player>("player");
    }

    // プレイヤーが見つからなかったらエラー回避
    if (!player) return false;
   

    /**SpringCameraの初期化*/
    m_springCamera.Init(
		*g_camera3D,
		600.0f,	// カメラの距離
        true,
		1.0f	// カメラの高さ
    );
	m_springCamera.SetNear(1.0f);
	m_springCamera.SetFar(10000.0f);

    /**初期注意点・視点*/
	Vector3 target = player->position;
	target.y += 60.0f;


    Vector3 startPos = target + m_toCameraPos;


	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(startPos);
    /** カメラのニアクリップとファークリップを設定*/
    /*g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);*/
    // ★重要: メインカメラ(g_camera3D)も強制的にスタート地点へワープさせる
    // これをしないと、リトライ前の場所にカメラが残った状態からスタートしてしまいます
    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(startPos);
    g_camera3D->Update(); // ここで確定させる
    return true;
}

/**
 * @brief カメラの更新処理
 * @details プレイヤーの位置を基準に注視点・視点を計算し、カメラに設定します。
 */
void GameCamera::Update()
{
    if (Game::IsPaused) return;


    /** カメラを更新*/
    /** 注視点を計算*/
    Vector3 target = player->position;
    /** プレイヤーの足元から少し上を注視点とする*/
    target.y += 60.0f;

    target += g_camera3D->GetForward() * 20.0f;

    Vector3 toCameraPosOld = m_toCameraPos;
    //パッドの入力を使ってカメラを回す。
    float x = g_pad[0]->GetRStickXF();
    float y = g_pad[0]->GetRStickYF();
    //Y軸周りの回転
    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, 1.0f * x);
    qRot.Apply(m_toCameraPos);
    //X軸周りの回転。
    Vector3 axisX;
    axisX.Cross(Vector3::AxisY, m_toCameraPos);
    axisX.Normalize();
    qRot.SetRotationDeg(axisX, 1.0 * y);
    qRot.Apply(m_toCameraPos);
    //カメラの回転の上限をチェックする。
    //注視点から視点までのベクトルを正規化する。
    //正規化すると、ベクトルの大きさが１になる。
    //大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
    Vector3 toPosDir = m_toCameraPos;
    toPosDir.Normalize();
    if (toPosDir.y < -0.9f) {
        //カメラが上向きすぎ。
        m_toCameraPos = toCameraPosOld;
    }
    else if (toPosDir.y > 0.9f) {
        //カメラが下向きすぎ。
        m_toCameraPos = toCameraPosOld;
    }
    /** 視点を計算*/
    Vector3 pos = target + m_toCameraPos;
    ///** メインカメラに注視点と視点を設定*/
    //g_camera3D->SetTarget(target);
    //g_camera3D->SetPosition(pos);

    ///** カメラの更新*/
    //g_camera3D->Update();
	/** スプリングカメラに注視点と視点を設定*/
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(target+m_toCameraPos);

	/** スプリングカメラの更新*/
	m_springCamera.Update();
}