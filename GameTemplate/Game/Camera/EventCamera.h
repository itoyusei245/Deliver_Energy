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
    * @brief ボス撃破時ルートをセットする関数
    */
    void LoadClearPath();

    // どこからでもアクセスできる「イベント中フラグ」
    static bool IsEventPlaying;

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

    // イベント終了時の処理をまとめる専用関数
    void FinishEvent();

private:
    std::vector<CameraKeyFrame>m_pathList;


    bool m_isPlaying        = false; // 再生中フラグ
    bool m_isCutMode        = false; ;// カット切り替えモードかどうか
    bool m_isClearMode      = false; // 撃破演出かどうか
    int m_currentIndex      =     0; // 現在どの地点(A->BのA)にいるか
    float m_segmentTimer    =  0.0f; // 現在の区間の経過時間
    float m_segmentDuration =  0.0f; // 1区間を何秒で移動するか
};