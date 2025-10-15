#pragma once
#include"camera/SpringCamera.h"

class Player;

/**
 * @brief ゲーム中のカメラを制御するクラス
 * @details プレイヤーを追従し、カメラの位置・注視点を管理します。
 */
class GameCamera : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details カメラオブジェクトの初期化を行います。
     */
    GameCamera();

    /**
     * @brief デストラクタ
     * @details カメラオブジェクトの終了処理を行います。
     */
    ~GameCamera();

    /**
     * @brief カメラの初期化処理
     * @return 初期化が成功した場合はtrue
     */
    bool Start();

    /**
     * @brief カメラの更新処理
     * @details プレイヤーの位置に応じてカメラの位置・注視点を更新します。
     */
    void Update();

    /** @brief 追従対象のプレイヤー */
    Player* player= nullptr;

    /** @brief 注視点から視点に向かうベクトル */
    Vector3 m_toCameraPos = Vector3::One;
private:
	SpringCamera m_springCamera; /**< スプリングカメラオブジェクト */
};

