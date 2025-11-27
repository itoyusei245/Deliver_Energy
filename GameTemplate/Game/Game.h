#pragma once
#include "Level3DRender/LevelRender.h"
class CoinUI;
class GameTimeUI;
class Countdown;
class Player;
class NoobEnemy;
class GameCamera;
class BackGround;
class EnemyManager;
class StageManager;

/**
 * @brief ゲームシーン管理クラス
 * @details ゲームシーン内の各オブジェクト（プレイヤー、敵、カメラ、背景など）を管理します。
 */
class Game : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details ゲームシーンの初期化を行います。
     */
    Game();

    /**
     * @brief デストラクタ
     * @details ゲームシーンの終了処理を行います。
     */
    ~Game();

    /**
     * @brief ゲーム開始時の初期化処理
     * @return 初期化が成功した場合はtrue
     */
    bool Start();

    /**
     * @brief 毎フレームの更新処理
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @details ゲームシーンの描画を行います。
     */
    void Render(RenderContext& rc);

    static bool IsGamePlay;
    static int CoinCount;

private:

    /** @brief ゲームシーンのモデルレンダラー */
    ModelRender m_modelRender;

    /** @brief ゲームシーンの座標 */
    Vector3 m_pos = Vector3::Zero;

    /** @brief プレイヤーオブジェクト */
    Player* m_player = nullptr;

    /** @brief 敵オブジェクト */
    NoobEnemy* m_noobEnemy = nullptr;

    /** @brief ゲームカメラオブジェクト */
    GameCamera* m_gameCamera;

    /** @brief 背景オブジェクト */
    BackGround* m_backGround;

    /** @brief 汎用座標変数 */
    Vector3 position;

    Countdown* m_countdown = nullptr;

    GameTimeUI* m_gameTimeUI = nullptr;

    CoinUI* m_coinUI = nullptr;

    /** @brief スプライトレンダラー */
    SpriteRender spriteRender;
};