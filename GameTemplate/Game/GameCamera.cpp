#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

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
    m_toCameraPos.Set(450.0f, 0.0f, 0.0f);
    /** プレイヤーのインスタンスを探す*/
    player = FindGO<Player>("player");

    /** カメラのニアクリップとファークリップを設定*/
    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);

    return true;
}

/**
 * @brief カメラの更新処理
 * @details プレイヤーの位置を基準に注視点・視点を計算し、カメラに設定します。
 */
void GameCamera::Update()
{
    /** カメラを更新*/
    /** 注視点を計算*/
    Vector3 target = player->position;
    /** プレイヤーの足元から少し上を注視点とする*/
    target.y += 60.0f;

    Vector3 toCameraPosOld = m_toCameraPos;

    /** 視点を計算*/
    Vector3 pos = target + m_toCameraPos;
    /** メインカメラに注視点と視点を設定*/
    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(pos);

    /** カメラの更新*/
    g_camera3D->Update();
}