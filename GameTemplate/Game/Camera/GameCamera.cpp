/**
 * @file GameCamera.cpp
 * @brief ゲームカメラの実装
 */
#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include"Game.h"

namespace
{
    const Vector3 DEFAULT_CAMERA_POS(-450.0f, 0.0f, 0.0f);
	constexpr float CAMERA_TARGET_OFFSET_Y = 60.0f;
	constexpr float CAMERA_TARGET_OFFSET_Z = 20.0f;
	
    constexpr float CLIP_NEAR = 1.0f;
	constexpr float CLIP_FAR  = 15000.0f;

	constexpr float DEFAULT_ROT_SPEED_X = 1.0f;
	constexpr float DEFAULT_ROT_SPEED_Y = 1.0f;
	constexpr float PITCH_LIMIT_CLAMP   = 0.9f; 
}

 /**
  * @brief GameCameraクラスのコンストラクタ
  */
GameCamera::GameCamera()
{
	m_toCameraPos     = DEFAULT_CAMERA_POS;
	m_targetOffsetY   = CAMERA_TARGET_OFFSET_Y;
	m_targetOffsetZ   = CAMERA_TARGET_OFFSET_Z;
	m_rotSpeedX       = DEFAULT_ROT_SPEED_X;
	m_rotSpeedY       = DEFAULT_ROT_SPEED_Y;
	m_pitchLimitClamp = PITCH_LIMIT_CLAMP;
}

/**
 * @brief GameCameraクラスのデストラクタ
 */
GameCamera::~GameCamera()
{

}

/**
 * @brief カメラの初期化処理
 * @details
 * 1. カメラの相対位置ベクトルの初期化
 * 2. プレイヤーオブジェクトの検索・取得
 * 3. SpringCamera（遅延追従カメラ）の初期化（距離、コリジョン有無、高さ）
 * 4. クリップ平面（Near/Far）の設定
 * 5. **重要**: エンジンのメインカメラ(g_camera3D)を強制的に初期位置へ移動させ、
 * シーン遷移直後の描画乱れ（前の場所から飛んでくる現象）を防ぎます。
 * @return プレイヤーが見つからない場合はfalseを返します。
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
        0.0f,	// カメラの距離
        true,   // カメラコリジョン（壁めり込み防止）を有効化
        1.0f	// カメラの注視点オフセット（高さ）
    );
    m_springCamera.SetNear(CLIP_NEAR);
    m_springCamera.SetFar(CLIP_FAR);

    /**初期注視点の計算*/
    Vector3 target = player->position;
    target.y += m_targetOffsetY; // 足元ではなく少し上を見る

    /** 初期カメラ位置の計算*/
    Vector3 startPos = target + m_toCameraPos;

    // スプリングカメラに初期値をセット
    m_springCamera.SetTarget(target);
    m_springCamera.SetPosition(startPos);

    /** カメラのニアクリップとファークリップを設定*/
    /*g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);*/

    // メインカメラ(g_camera3D)も強制的にスタート地点へワープさせる
    // これをしないと、スプリングカメラが前回の位置から補間してしまい、
    // リトライ時などにカメラが高速移動する見た目になってしまうのを防ぐ。
    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(startPos);
    g_camera3D->Update(); // ここで確定させる

    return true;
}

/**
 * @brief カメラの更新処理
 * @details
 * 1. ポーズ中は更新を停止。
 * 2. プレイヤー位置を基準に注視点(target)を決定。
 * 3. 右スティック入力(RStick)を取得し、カメラ位置ベクトル(m_toCameraPos)を回転。
 * - X軸入力：Y軸回転（左右）
 * - Y軸入力：カメラの横ベクトル軸回転（上下）
 * 4. 上下の回転角度制限（クランプ）：真上・真下に行き過ぎないよう制御。
 * 5. 計算結果をSpringCameraに渡し、滑らかな追従を実行。
 */
void GameCamera::Update()
{
    // 無効なら更新せずに帰る
    if (!m_isEnable) return;

    if (Game::IsPaused) return;


    /** カメラを更新*/
    /** 注視点を計算*/
    Vector3 target = player->position;
    /** プレイヤーの足元から少し上(60.0f)を注視点とする*/
    target.y += m_targetOffsetY;

    // カメラのForwardを使って少し先を見るなどの処理（現在はコメントアウト的な扱いか、補正用）
    target += g_camera3D->GetForward() * m_targetOffsetZ;

    Vector3 toCameraPosOld = m_toCameraPos;

    // パッドの右スティック入力を使ってカメラを回す
    float x = g_pad[0]->GetRStickXF();
    float y = g_pad[0]->GetRStickYF();

    // --- Y軸周りの回転（左右回転） ---
    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, m_rotSpeedX * x);
    qRot.Apply(m_toCameraPos);

    // --- X軸（ローカル横軸）周りの回転（上下回転） ---
    Vector3 axisX;
    axisX.Cross(Vector3::AxisY, m_toCameraPos); // Y軸とカメラベクトルの外積で横軸を求める
    axisX.Normalize();
    qRot.SetRotationDeg(axisX, m_rotSpeedY * y);
    qRot.Apply(m_toCameraPos);

    // --- カメラの回転角度制限（クランプ） ---
    // 注視点から視点までのベクトルを正規化して向きを確認
    Vector3 toPosDir = m_toCameraPos;
    toPosDir.Normalize();

    // Y成分がm_pitchLimitClampを超えると真上/真下に近すぎるため、回転を取り消す（Oldに戻す）
    if (toPosDir.y < -m_pitchLimitClamp) {
        // カメラが上向きすぎ（見上げすぎ）
        m_toCameraPos = toCameraPosOld;
    }
    else if (toPosDir.y > m_pitchLimitClamp) {
        // カメラが下向きすぎ（見下ろしすぎ）
        m_toCameraPos = toCameraPosOld;
    }

    /** 視点を計算*/
    Vector3 pos = target + m_toCameraPos;

    // (直接g_camera3Dに入れるのではなく、SpringCameraを経由させる)
    // g_camera3D->SetTarget(target);
    // g_camera3D->SetPosition(pos);
    // g_camera3D->Update();

    /** スプリングカメラに注視点と目標視点位置を設定*/
    m_springCamera.SetTarget(target);
    // SpringCameraは「目標位置」に向かって徐々に移動する
    m_springCamera.SetPosition(target + m_toCameraPos);

    /** スプリングカメラの更新（内部でg_camera3Dに反映される）*/
    m_springCamera.Update();
}