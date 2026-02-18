/**
 * @file GameCamera.h
 * @brief ゲームプレイ中のカメラ制御クラス定義
 */
#pragma once
#include "camera/SpringCamera.h"

class Player;

/**
 * @class GameCamera
 * @brief プレイヤー追従型カメラ（TPS視点）クラス
 * @details プレイヤーの周囲を回転移動でき、SpringCameraを利用して滑らかに追従します。
 */
class GameCamera : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     */
    GameCamera();

    /**
     * @brief デストラクタ
     */
    ~GameCamera();

    /**
     * @brief 初期化処理
     * @details プレイヤーの参照取得、スプリングカメラのパラメータ設定、
     * および初期位置への強制ワープ（フレーム飛び防止）を行います。
     * @return bool 初期化成功ならtrue
     */
    bool Start();

    /**
     * @brief 更新処理
     * @details コントローラー入力による視点回転計算と、追従座標の更新を行います。
     */
    void Update();

    // カメラの有効/無効をセットする関数
    void SetEnable(bool isEnable) { m_isEnable = isEnable; }

    /** @brief 追従対象のプレイヤーへのポインタ */
    Player* player = nullptr;

    /** * @brief 注視点（プレイヤー）からカメラ位置への相対ベクトル
     * @details このベクトルを回転させることで、カメラのアングルを変更します。
     */
    Vector3 m_toCameraPos = Vector3::One;

private:
    /** * @brief スプリングカメラオブジェクト
     * @details 目標地点に対してバネのような動きで滑らかに追従する機能を提供します。
     */
    SpringCamera m_springCamera;
    
    // 有効フラグ（最初はtrue）
    bool m_isEnable = true;
};