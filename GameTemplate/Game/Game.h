/**
 * @file Game.h
 * @brief ゲームメインシーンのクラス定義
 */
#pragma once
#include "Level3DRender/LevelRender.h"

 // 前方宣言
class CoinUI;
class GameTimeUI;
class Countdown;
class Player;
class NoobEnemy;
class GameCamera;
class BackGround;
class EnemyManager;
class StageManager;
class CollisionHitManagerObject;
class GameUI;

/**
 * @class Game
 * @brief ゲーム本編の進行管理クラス
 * @details プレイヤー、カメラ、敵、UI、各種マネージャクラスの生成・破棄・更新を統括します。
 * IGameObjectを継承しており、エンジンのゲームループ内で駆動します。
 */
class Game : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     * @details プレイヤーやカメラなど、基本となるオブジェクトのメモリ確保を行います。
     */
    Game();

    /**
     * @brief デストラクタ
     * @details ゲーム終了時に呼び出され、各オブジェクトやシングルトンインスタンスの破棄を行います。
     */
    ~Game();

    /**
     * @brief ゲーム開始処理
     * @details 各種UI、マネージャ(Enemy, Stage, Athletic)、背景などの生成と初期化(Setup)を実行します。
     * @return bool 常にtrue
     */
    bool Start();

    /**
     * @brief 更新処理
     * @details カウントダウンの監視、ゲームプレイフラグの制御、各マネージャの更新を行います。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     * @note Gameクラス自体は特定のモデルを持たないため、ここでの描画処理は基本的に空です。
     */
    void Render(RenderContext& rc);

    // --- 静的メンバ変数（グローバルな状態管理） ---

    /** ゲームがプレイ中かどうか（カウントダウン終了後true、ポーズ中などはfalseの可能性あり） */
    static bool IsGamePlay;

    /** 獲得コイン数（GetItemクラス等で参照） */
    static int CoinCount;

    /** ポーズ中かどうか */
    static bool IsPaused;

private:

    /** ゲームシーン用のモデルレンダラー（※現状未使用の可能性あり） */
    ModelRender m_modelRender;

    /** ゲームシーンの基準座標 */
    Vector3 m_pos = Vector3::Zero;

    /** プレイヤーオブジェクト */
    Player* m_player = nullptr;

    /** 敵オブジェクト（※個別に保持しているが、基本はManager管理） */
    NoobEnemy* m_noobEnemy = nullptr;

    /** ゲームカメラ */
    GameCamera* m_gameCamera;

    /** 背景オブジェクト（冷蔵庫ギミック等含む） */
    BackGround* m_backGround;

    /** 汎用座標変数 */
    Vector3 position;

    /** カウントダウン演出UI */
    Countdown* m_countdown = nullptr;

    /** 経過時間表示UI */
    GameTimeUI* m_gameTimeUI = nullptr;

    /** コイン枚数表示UI */
    CoinUI* m_coinUI = nullptr;

    /** HPゲージ管理用UI */
    GameUI* m_gameUI = nullptr;

    /** スプライトレンダラー */
    SpriteRender spriteRender;

    /** 当たり判定管理用オブジェクト（Managerのラッパー） */
    CollisionHitManagerObject* m_collisionHitManagerObject = nullptr;
};