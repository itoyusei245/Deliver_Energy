#pragma once
#include <Vector>


/**
 * @file GameCamera.h
 * @brief ゲームプレイ中のカメラ制御クラス定義
 */


/**
 * @class EventCamera
 * @brief イベントカメラクラス
 */
class EventCamera : public IGameObject
{
public:
    struct CameraKeyFrame{
        Vector3 position; //本体地点
        Vector3 target;   //視点位置
    };

public:
    /**
     * @brief コンストラクタ
     */
    EventCamera();

    /**
     * @brief デストラクタ
     */
    ~EventCamera();

    /**
     * @brief 初期化処理
     */
    bool Start();

    /**
     * @brief 更新処理
     */
    void Update();

    /**
     * @brief イベント再生開始
     */
    void Play();

    /**
     * @brief ボスの座標ルートをセットする関数
     */
    void LoadBossPath();

    /**
     * @brief 再生中かどうか
     */
    bool IsPlaying()const { return m_isPlaying; }


private:
    /**
     * @brief イージング関数(SmoothStep)
     * 0.0f～1.0fの進行度
     */
    float EaseInOut(float t);


private:
    std::vector<CameraKeyFrame>m_pathList;


    bool m_isPlaying = false;    // 再生中フラグ
    bool m_isCutMode = false;    // カット切り替えモードかどうか
    int m_currentIndex = 0;      // 現在どの地点(A->BのA)にいるか
    float m_segmentTimer = 0.0f; // 現在の区間の経過時間


    // 1区間を何秒で移動するか
    float m_segmentDuration = 3.0f;
};