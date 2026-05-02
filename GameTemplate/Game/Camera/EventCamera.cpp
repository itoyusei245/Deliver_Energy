/**
 * @file EventCamera.cpp
 * @brief イベントカメラの実装
 */
#include "stdafx.h"
#include "EventCamera.h"
#include "Game.h"

namespace {
	constexpr float DURATION_OPENING_SEGMENT =  3.0f; // オープニングの各区間の時間
	constexpr float DURATION_BOSS_SEGMENT    =  1.5f; // ボス戦の各区間の時間
	constexpr float DURATION_BOSS_LAST_WAIT  =  6.0f; // ボス戦の最後の区間の時間 
	constexpr float DURATION_CLEAR_SEGMENT   =  1.5f; // 撃破演出の各区間の時間
	constexpr float DURATION_CLEAR_WAIT      =  4.0f; // 撃破演出の最後の区間の時間
	constexpr float SHAKE_POWER_CLEAR        = 10.0f; // 撃破演出のカメラシェイクの強さ

    const std::vector<EventCamera::CameraKeyFrame>PATH_OPENING = {
        {Vector3( 285.4f, -268.0f,   -55.4f),      Vector3( 425.4f, -338.2f,  -189.4f)},
        {Vector3( 557.8f, 1246.0f,   -79.0f),      Vector3( 578.7f, 1070.7f,  -185.4f)},
        {Vector3(1266.0f, 1360.5f,  -201.3f),      Vector3(1295.1f, 1198.6f,  -325.6f)},
        {Vector3(1378.6f,  220.0f,    81.2f),      Vector3(1432.5f,   43.0f,    -9.8f)},
        {Vector3(1496.8f, -267.1f,   -85.0f),      Vector3(1582.9f, -279.7f,  -271.9f)},
        {Vector3(1940.4f, -311.4f, -1176.9f),      Vector3(2025.0f, -289.6f, -1363.6f)},
        {Vector3(2517.5f, -219.2f, -2238.6f),      Vector3(2631.7f, -209.3f, -2410.0f)},
        {Vector3(3043.5f,  283.0f, -2610.2f),      Vector3(3208.1f,  235.1f, -2725.1f)},
        {Vector3(3327.3f,  262.3f, -3388.6f),      Vector3(3132.1f,  201.0f, -3413.3f)},
        {Vector3(3606.0f,   78.6f, -4727.5f),      Vector3(3638.1f,   43.0f, -4928.0f)},
        {Vector3(2607.5f,   34.9f, -5108.8f),      Vector3(2401.5f,   33.0f, -5099.2f)},
        {Vector3(1190.7f,  -32.1f, -5157.8f),      Vector3( 984.5f,  -32.4f, -5161.2f)},
        {Vector3(1059.4f,  -27.6f, -4889.4f),      Vector3(1020.2f,  -22.7f, -4687.1f)},
        {Vector3(1322.1f,  831.6f, -4417.0f),      Vector3(1224.5f,  729.6f, -4266.7f)},
        {Vector3(1684.2f, 1071.7f, -3133.2f),      Vector3(1507.9f,  971.9f, -3095.2f)},
		{Vector3( 980.0f,  621.8f, -2895.7f),      Vector3( 806.7f,  510.2f, -2893.2f)},
        {Vector3( 212.9f,  195.4f, -2894.2f),      Vector3(  31.5f,   99.4f, -2875.1f)}
    };

    const std::vector<EventCamera::CameraKeyFrame>PATH_BOSS = {
        { Vector3(2695.4f,  -75.4f, -3509.0f),  Vector3(2825.4f,  -0.4f, -3650.3f) },
        { Vector3(4225.8f,  562.6f, -4591.5f),  Vector3(4091.4f, 476.2f, -4461.1f) },
        { Vector3(4033.7f,  953.6f, -3466.0f),  Vector3(3921.0f, 822.1f, -3587.8f) },
        { Vector3(2993.4f, -166.8f, -2807.2f),  Vector3(2889.7f, -50.2f, -2908.9f) }
    };

    const std::vector<EventCamera::CameraKeyFrame>PATH_CLEAR = {
        { Vector3(2501.10f, 1220.76f, -4314.70f), Vector3(2666.08f, 1100.59f, -4285.78f) },
        { Vector3(2505.74f,  925.14f, -3428.83f), Vector3(2656.39f,  835.40f, -3537.21f) },
        { Vector3(3191.28f,  384.00f, -3050.08f), Vector3(3224.34f,  374.16f, -3253.31f) },
        { Vector3(3219.76f,  -43.21f, -3045.79f), Vector3(3248.05f,   37.79f, -3233.23f) }
    };
}

bool EventCamera::IsEventPlaying = false;

 /**
  * @brief EventCameraクラスのコンストラクタ
  */
EventCamera::EventCamera()
{
    m_pathList = PATH_OPENING;
    m_segmentDuration = DURATION_OPENING_SEGMENT;

    m_isPlaying     = false;
    m_isCutMode     = false;
    m_isClearMode   = false;
    m_currentIndex  =     0;
    m_segmentTimer  =  0.0f;
}

/**
 * @brief EventCameraクラスのデストラクタ
 */
EventCamera::~EventCamera()
{

}

/**
 * @brief カメラの初期化処理
 * @details
 */
bool EventCamera::Start()
{
    return true;
}

void EventCamera::Play()
{
    if (m_pathList.size() < 2)return;


    m_isPlaying = true;
    
    // イベント中フラグをON
    IsEventPlaying = true;

    m_currentIndex = 0;
    m_segmentTimer = 0.0f;


    // 最初の地点にカメラをワープさせておく
    g_camera3D->SetPosition(m_pathList[0].position);
    g_camera3D->SetTarget(m_pathList[0].target);
    g_camera3D->Update();

    // ボス戦モードなら、最初のカウントダウンを鳴らす
    if (m_isCutMode)
    {
        SoundManager::Get().PlaySE(enSoundKind_EnemyCount);
    }
}


// ---------------------------------------------------
// 終了処理をまとめた関数
// ---------------------------------------------------
void EventCamera::FinishEvent()
{
    m_isPlaying = false;

    // イベント中フラグをOFF
    IsEventPlaying = false;

    // ボス戦なら、終わった瞬間にBGMを流す
    if (m_isCutMode) {
        SoundManager::Get().PlayBGM(enSoundKind_Boss);
    }
}


// Catmull-Romスプライン補間関数
// p0: 1つ前の点, p1: 始点, p2: 終点, p3: 翌々の点, t: 進行度(0.0~1.0)
Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
    Vector3 a = p1 * 2.0f;
    Vector3 b = p2 - p0;
    Vector3 c = p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3;
    Vector3 d = (p0 * -1.0f) + p1 * 3.0f - p2 * 3.0f + p3;

    return (a + (b * t) + (c * t * t) + (d * t * t * t)) * 0.5f;
}


/**
 * @brief カメラの更新処理
 * @details
 */
void EventCamera::Update()
{
    if (!m_isPlaying) return;

    // Aボタンが押されたらイベントをスキップ
    if (g_pad[0]->IsTrigger(enButtonA)) {
        FinishEvent(); // 終了処理を呼ぶ
        return;
    }

    if (m_pathList.empty()) {
        m_isPlaying = false;
        return;
    }

    // 次の地点へのインデックス
    int nextIndex = m_currentIndex + 1;

    if (!m_isCutMode && !m_isClearMode && nextIndex >= m_pathList.size())
    {
        FinishEvent();
        return;
    }

    // 今回の待ち時間を決定する
    float currentWaitTime = m_segmentDuration;

    // もし「カットモード」かつ「最後の1枚」なら、時間を6.0秒にする
    if (m_isCutMode && m_currentIndex == (m_pathList.size() - 1))
    {
        currentWaitTime = DURATION_BOSS_LAST_WAIT;
    }
    else if (m_isClearMode && nextIndex >= m_pathList.size())
    {
        currentWaitTime = DURATION_CLEAR_WAIT;
    }

    // 時間経過
    m_segmentTimer += g_gameTime->GetFrameDeltaTime();

    // 進行度
    float rate = m_segmentTimer / currentWaitTime;

    // 時間が来たら次のカットへ
    if (rate >= 1.0f)
    {
        rate = 1.0f;
        m_currentIndex++;
        m_segmentTimer = 0.0f;

        //　終了判定
        if (m_currentIndex >= m_pathList.size()) {

            FinishEvent();
            return;
        }

        // ---------------------------------------------------
        // 切り替わった瞬間のサウンド再生
        // ---------------------------------------------------
        if (m_isCutMode)
        {
            // 最後のカット（4つ目）ならボイス
            if (m_currentIndex == m_pathList.size() - 1)
            {
                SoundManager::Get().PlaySE(enSoundKind_EnemyVoice);
            }
            // それ以外（1, 2つ目など）ならカウントダウン
            else
            {
                SoundManager::Get().PlaySE(enSoundKind_EnemyCount);
            }
        }
    }

    // ---------------------------------------------------
    // ポイント: モードによる分岐
    // ---------------------------------------------------
    if (m_isCutMode)
    {
        // 補間（移動）は一切せず、現在の地点に固定する
        const auto& currentFrame = m_pathList[m_currentIndex];

        g_camera3D->SetPosition(currentFrame.position);
        g_camera3D->SetTarget(currentFrame.target);
        g_camera3D->Update();
    }
    else
    {
        // クリアモードの待機中（最後の点に到達している場合）はシェイクさせる
        if (m_isClearMode && nextIndex >= m_pathList.size())
        {
            const auto& lastFrame = m_pathList[m_pathList.size() - 1];

            // ---------------------------------------------------
            // カメラシェイク処理
            // ---------------------------------------------------
            float shakePower = SHAKE_POWER_CLEAR; // 揺れの強さ

            Vector3 shakeOffset;
            // -shakePower ～ +shakePower の間でランダムなズレを作る
            shakeOffset.x = ((rand() % 100) / 100.0f * 2.0f - 1.0f) * shakePower;
            shakeOffset.y = ((rand() % 100) / 100.0f * 2.0f - 1.0f) * shakePower;
            shakeOffset.z = ((rand() % 100) / 100.0f * 2.0f - 1.0f) * shakePower;

            // 元の座標にランダムなズレ（オフセット）を足して設定する
            g_camera3D->SetPosition(lastFrame.position + shakeOffset);

            // ターゲット（注視点）も一緒に揺らさないと不自然になるため、同じズレを足す
            g_camera3D->SetTarget(lastFrame.target + shakeOffset);

            g_camera3D->Update();
        }
        else
        {
            // オープニング用
            // Catmull-Rom補間

            float t = rate;

            // 4点取得のロジック
            int i0 = (m_currentIndex - 1) < 0 ? m_currentIndex : (m_currentIndex - 1);
            int i1 = m_currentIndex;
            int i2 = nextIndex;
            int i3 = (nextIndex + 1) >= m_pathList.size() ? nextIndex : (nextIndex + 1);

            const auto& p0 = m_pathList[i0];
            const auto& p1 = m_pathList[i1];
            const auto& p2 = m_pathList[i2];
            const auto& p3 = m_pathList[i3];

            Vector3 currentPos = CatmullRom(p0.position, p1.position, p2.position, p3.position, t);
            Vector3 currentTarget = CatmullRom(p0.target, p1.target, p2.target, p3.target, t);

            g_camera3D->SetPosition(currentPos);
            g_camera3D->SetTarget(currentTarget);
            g_camera3D->Update();
        }
    }
}


//シンプルなイージング関数
float EventCamera::EaseInOut(float t)
{
    return t * t * (3.0f - 2.0f * t);
}


void EventCamera::LoadBossPath()
{
	m_pathList        =             PATH_BOSS;
    m_isCutMode       =                  true;
	m_segmentDuration = DURATION_BOSS_SEGMENT;
}


void EventCamera::LoadClearPath()
{
    m_pathList        =             PATH_CLEAR;
	m_isCutMode       =                  false;
    m_isClearMode     =                   true;
	m_segmentDuration = DURATION_CLEAR_SEGMENT;
}