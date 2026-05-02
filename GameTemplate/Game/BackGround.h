/**
 * @file BackGround.h
 * @brief ステージ背景オブジェクトのクラス定義
 */
#pragma once

 /**
  * @class BackGround
  * @brief 背景オブジェクトクラス
  * @details ステージの静的なモデル表示、当たり判定（Physics）、および
  * 冷蔵庫のドアが開くアニメーション（ヒンジ回転制御）を管理します。
  */
class BackGround : public IGameObject
{
public:
    /**
     * @brief コンストラクタ
     */
    BackGround();

    /**
     * @brief デストラクタ
     */
    ~BackGround();

    /**
     * @brief 更新処理
     * @details ゲームプレイ中、冷蔵庫のドアを開くアニメーション処理を行います。
     */
    void Update();

    /**
     * @brief 描画処理
     * @param rc 描画コンテキスト
     */
    void Render(RenderContext& rc);

    void Init();

    /** * @brief 背景の物理静的オブジェクト
     * @note プレイヤーや敵が乗るための当たり判定です。
     */
    PhysicsStaticObject physicsStaticObject;
    PhysicsStaticObject m_physicsState; // カウントダウン用
    PhysicsStaticObject m_physicsBoss;  // ボス戦用

private:
    /** 背景モデルのレンダラー */
    ModelRender m_map;
    /** 当たり判定用の透明モデル */
    ModelRender m_hitBox;
    /** アニメーションするモデル */
    ModelRender m_anim;
    ModelRender m_pro;
    ModelRender m_blockState;
    ModelRender m_brockBoss;

    /** * @brief 現在の開閉角度
     * @note 0度から始まり、マイナス方向へ開いていきます。
     */
    float m_openAngle = 0.0f;

    /**
     * @brief アニメーションモデルの初期位置
     * @note 回転補正計算の基準点として使用します。
     */
    Vector3 m_initialPos = Vector3::Zero;

    /**
     * @brief ヒンジ（回転軸）へのオフセット
     * @note モデルの中心点から見た、蝶番（ヒンジ）の位置ベクトルです。
     * この値を元に「中心回転」ではなく「端での回転」を計算します。
     */
    Vector3 m_hingeOffset = Vector3::Zero;
};