/**
 * @file BackGround.cpp
 * @brief 背景オブジェクトの実装
 */
#include "stdafx.h"
#include "BackGround.h"
#include"Game.h"

 /**
  * @brief BackGroundクラスのコンストラクタ
  * @details
  * 1. 背景モデル(map.tkm)の読み込み
  * 2. アニメーションモデル(reizouko_anim.tkm)の読み込みとヒンジ位置の設定
  * 3. 当たり判定用モデル(map_HitBox.tkm)から物理メッシュを生成
  * 4. デバッグ用ワイヤーフレームの有効化
  */
BackGround::BackGround()
{
    /** モデルの位置を設定*/
    m_map.SetPosition(0.0f, 0.0f, 0.0f);
    /** 背景モデルを初期化*/
    m_map.Init("Assets/modelData/Stage/map.tkm");

    // アニメーション用モデル（冷蔵庫）の初期化
    m_anim.SetPosition(0.0f, 0.0f, 0.0f);
    m_anim.Init("Assets/modelData/Stage/reizouko_anim.tkm");

    // ヒンジ（回転軸）のローカル座標オフセットを設定
    m_hingeOffset = Vector3(250.0f, 0.0f, 220.0f);

    m_openAngle = 0.0f;

    // 当たり判定用モデルのロード
    m_hitBox.SetPosition(0.0f, 0.0f, 0.0f);
    m_hitBox.Init("Assets/modelData/Stage/map_HitBox.tkm");
    m_hitBox.Update();

    /** 物理静的オブジェクトをモデルから生成*/
    physicsStaticObject.CreateFromModel(m_hitBox.GetModel(), m_hitBox.GetModel().GetWorldMatrix());

    /** 物理静的オブジェクトをモデルから生成（コメントアウト中）*/
    //physicsStaticObject.CreateFromModel(m_mapAthletic.GetModel(), m_mapAthletic.GetModel().GetWorldMatrix());

    /** デバッグ用ワイヤーフレーム表示を有効化*/
    PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

/**
 * @brief BackGroundクラスのデストラクタ
 */
BackGround::~BackGround()
{
}

/**
 * @brief 更新処理
 * @details
 * Game::IsGamePlayフラグが立っている間、冷蔵庫のドアを開くアニメーションを実行します。
 * 単純な回転ではなく、ヒンジ位置（m_hingeOffset）を基準とした円軌道を描くように
 * 座標と回転を同時に計算・適用しています。
 */
void BackGround::Update()
{
    if (Game::IsGamePlay)
    {
        // 指定の角度（-51度）になるまで開き続ける
        if (m_openAngle > -51.0f)
        {
            // 開くスピード (1秒間に25度動く)
            float openSpeed = -25.0f;

            // 経過時間分だけ角度を足す
            m_openAngle += openSpeed * g_gameTime->GetFrameDeltaTime();

            // 行き過ぎないように-51度で止める（クランプ処理）
            if (m_openAngle < -51.0f) {
                m_openAngle = -51.0f;
            }

            // 回転クォータニオンを作成 (Y軸を中心に回転)
            Quaternion rot;
            rot.SetRotationDeg(Vector3::AxisY, m_openAngle);
            m_anim.SetRotation(rot);

            // --- ヒンジを中心とした回転座標の計算 ---

            // 1. 本来のヒンジのオフセットベクトルを、現在の角度で回転させる
            Vector3 currentOffset = m_hingeOffset;
            rot.Apply(currentOffset);

            // 2. 「回転前のオフセット」と「回転後のオフセット」の差分（ズレ）を計算
            // これが「中心回転」から「ヒンジ回転」にするための位置補正値となる
            Vector3 positionCorrection = m_hingeOffset - currentOffset;

            // 3. 初期位置に補正値を足してセットすることで、見かけ上ヒンジを中心に回っているように見せる
            m_anim.SetPosition(m_initialPos + positionCorrection);
        }
    }

    // 各モデルの更新
    m_map.Update();
    m_anim.Update();
}

/**
 * @brief 背景の描画処理
 * @param rc 描画コンテキスト
 * @details 背景モデルとアニメーションモデルの描画を行います。
 */
void BackGround::Render(RenderContext& rc)
{
    m_map.Draw(rc);
    m_anim.Draw(rc);
}