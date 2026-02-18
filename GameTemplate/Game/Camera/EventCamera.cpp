/**
 * @file EventCamera.cpp
 * @brief イベントカメラの実装
 */
#include "stdafx.h"
#include "EventCamera.h"
#include "Game.h"

 /**
  * @brief EventCameraクラスのコンストラクタ
  */
EventCamera::EventCamera()
{

    // ---------------------------------------------------
    // 指定された座標データの登録
    // A～Vまでの地点をリストに追加(x,z,y)
    // ---------------------------------------------------
    m_pathList.clear(); 

    m_pathList.push_back({ Vector3( 285.4f, -268.0f,   -55.4f),      Vector3( 425.4f, -338.2f,  -189.4f) });
    m_pathList.push_back({ Vector3( 557.8f, 1246.0f,   -79.0f),      Vector3( 578.7f, 1070.7f,  -185.4f) });
    m_pathList.push_back({ Vector3(1266.0f, 1360.5f,  -201.3f),      Vector3(1295.1f, 1198.6f,  -325.6f) });
    m_pathList.push_back({ Vector3(1378.6f,  220.0f,    81.2f),      Vector3(1432.5f,   43.0f,    -9.8f) });
    m_pathList.push_back({ Vector3(1496.8f, -267.1f,   -85.0f),      Vector3(1582.9f, -279.7f,  -271.9f) });
    m_pathList.push_back({ Vector3(1940.4f, -311.4f, -1176.9f),      Vector3(2025.0f, -289.6f, -1363.6f) });
    m_pathList.push_back({ Vector3(2517.5f, -219.2f, -2238.6f),      Vector3(2631.7f, -209.3f, -2410.0f) });
    m_pathList.push_back({ Vector3(3043.5f,  283.0f, -2610.2f),      Vector3(3208.1f,  235.1f, -2725.1f) });
    m_pathList.push_back({ Vector3(3327.3f,  262.3f, -3388.6f),      Vector3(3132.1f,  201.0f, -3413.3f) });
    m_pathList.push_back({ Vector3(3606.0f,   78.6f, -4727.5f),      Vector3(3638.1f,   43.0f, -4928.0f) });
    m_pathList.push_back({ Vector3(2607.5f,   34.9f, -5108.8f),      Vector3(2401.5f,   33.0f, -5099.2f) });
    m_pathList.push_back({ Vector3(1190.7f,  -32.1f, -5157.8f),      Vector3( 984.5f,  -32.4f, -5161.2f) });
    m_pathList.push_back({ Vector3(1059.4f,  -27.6f, -4889.4f),      Vector3(1020.2f,  -22.7f, -4687.1f) });
    m_pathList.push_back({ Vector3(1322.1f,  831.6f, -4417.0f),      Vector3(1224.5f,  729.6f, -4266.7f) });
    m_pathList.push_back({ Vector3(1684.2f, 1071.7f, -3133.2f),      Vector3(1507.9f,  971.9f, -3095.2f) });
    m_pathList.push_back({ Vector3( 980.0f,  621.8f, -2895.7f),      Vector3( 806.7f,  510.2f, -2893.2f) });
    m_pathList.push_back({ Vector3( 212.9f,  195.4f, -2894.2f),      Vector3(  31.5f,   99.4f, -2875.1f) });
    

    m_segmentDuration = 3.0f;
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
    if (m_pathList.empty()) {
        m_isPlaying = false;
        return;
    }

    // 次の地点へのインデックス
    int nextIndex = m_currentIndex + 1;

    if (!m_isCutMode && nextIndex >= m_pathList.size())
    {
        m_isPlaying = false;
        return;
    }

    // 今回の待ち時間を決定する
    float currentWaitTime = m_segmentDuration;

    // もし「カットモード」かつ「最後の1枚」なら、時間を6.0秒にする
    if (m_isCutMode && m_currentIndex == (m_pathList.size() - 1))
    {
        currentWaitTime = 6.0f;
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

            // ボス戦モードなら、すべて終わったここでBGM開始！
            if (m_isCutMode) {
                SoundManager::Get().PlayBGM(enSoundKind_Boss);
            }

            m_isPlaying = false;
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
        // ▼ カットモード（ボス用）
        // 補間（移動）は一切せず、現在の地点に固定する
        const auto& currentFrame = m_pathList[m_currentIndex];

        g_camera3D->SetPosition(currentFrame.position);
        g_camera3D->SetTarget(currentFrame.target);
        g_camera3D->Update();
    }
    else
    {
        // ▼ 滑らかモード（オープニング用）
        // これまで通りのCatmull-Rom補間

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


//シンプルなイージング関数
float EventCamera::EaseInOut(float t)
{
    return t * t * (3.0f - 2.0f * t);
}


void EventCamera::LoadBossPath()
{
    // 今入っている（オープニングの）データを空にする
    m_pathList.clear();

    // ボス戦は「パッと切り替える」モードにする
    m_isCutMode = true;

    m_segmentDuration = 1.5f;

    m_pathList.push_back({ Vector3(2695.4f,  -75.4f, -3509.0f),  Vector3(2825.4f,  -0.4f, -3650.3f) });
    m_pathList.push_back({ Vector3(4225.8f,  562.6f, -4591.5f),  Vector3(4091.4f, 476.2f, -4461.1f) });
    m_pathList.push_back({ Vector3(4033.7f,  953.6f, -3466.0f),  Vector3(3921.0f, 822.1f, -3587.8f) });
    m_pathList.push_back({ Vector3(2993.4f, -166.8f, -2807.4f),  Vector3(3057.9f, -97.6f, -2990.6f) });

    
   /* m_pathList.push_back({ Vector3(2984.6f, -278.015442f, -2566.1f), Vector3(2983.7f, -247.6f, -2770.0f) });
    m_pathList.push_back({ Vector3(2970.3f,  284.048401f, -4194.2f), Vector3(2969.2f,  337.0f, -4393.5f) });
    m_pathList.push_back({ Vector3(2127.5f, 1298.167236f, -5144.9f), Vector3(2271.6f, 1160.8f, -5091.3f) });
    m_pathList.push_back({ Vector3(2067.8f,  861.443787f, -4766.1f), Vector3(2248.5f,  871.6f, -4864.9f) });
    m_pathList.push_back({ Vector3(3071.9f,  632.125793f, -5874.1f), Vector3(3065.6f,  639.4f, -5668.2f) });
    m_pathList.push_back({ Vector3(3092.0f,  529.513184f, -3590.7f), Vector3(3144.0f,  532.2f, -3790.2f) });
    m_pathList.push_back({ Vector3(3070.5f,  473.511108f, -3345.4f), Vector3(3122.5f,  476.2f, -3544.8f) });
    m_pathList.push_back({ Vector3(3032.7f,  413.222260f, -3144.6f), Vector3(3084.8f,  415.9f, -3344.0f) });
    m_pathList.push_back({ Vector3(2986.4f,  362.671173f, -2956.7f), Vector3(3038.4f,  365.4f, -3156.1f) });
    m_pathList.push_back({ Vector3(2957.2f,  162.873367f, -2939.7f), Vector3(3007.2f,  188.6f, -3138.1f) });
    m_pathList.push_back({ Vector3(2929.0f,  140.827942f, -2371.6f), Vector3(2979.0f,  166.5f, -2570.0f) });
    m_pathList.push_back({ Vector3(2926.2f,  -77.529510f, -2303.0f), Vector3(2948.7f,  -22.2f, -2500.3f) });*/
}