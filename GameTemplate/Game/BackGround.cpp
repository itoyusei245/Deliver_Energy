/**
 * @file BackGround.cpp
 * @brief 背景オブジェクトの実装
 */
#include "stdafx.h"
#include "BackGround.h"
#include"Game.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss.h"

namespace 
{
    constexpr const char* PATH_MAP        = "Assets/modelData/Stage/map_new.tkm";
    constexpr const char* PATH_ANIM       = "Assets/modelData/Stage/reizouko_anim.tkm";
    constexpr const char* PATH_PROGRAMMER = "Assets/modelData/Stage/Programmer.tkm";
    constexpr const char* PATH_STATE      = "Assets/modelData/Stage/state.tkm";
    constexpr const char* PATH_BOSS       = "Assets/modelData/Stage/Bosss.tkm";

	constexpr float   DOOR_OPEN_SPEED   = -25.0f; // 1秒間に25度開く
	constexpr float   DOOR_TARGET_ANGLE = -51.0f; // ドアが完全に開いたときの角度
    
    const Vector3 POS(0.0f, 0.0f, 0.0f);
    const Vector3 HINGE_OFFSET   (250.0f, 0.0f, 220.0f);
	const Vector3 POS_HIDE_STATE (0.0f, -2000.0f, 0.0f );
	const Vector3 POS_HIDE_BOSS  (0.0f, -10000.0f, 0.0f );
}
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
    m_map.SetPosition(POS);
    m_map.Init(PATH_MAP);

    m_anim.SetPosition(POS);
    m_anim.Init(PATH_ANIM);

    m_pro.SetPosition(POS);
    m_pro.Init(PATH_PROGRAMMER);

    m_hitBox.SetPosition(POS);
    m_hitBox.Init(PATH_MAP);
    m_hitBox.Update();
    
    m_blockState.SetPosition(POS);
    m_blockState.Init(PATH_STATE);
    m_blockState.Update();

    m_brockBoss.SetPosition(POS);
    m_brockBoss.Init(PATH_BOSS);
    m_brockBoss.Update();

    m_hingeOffset = HINGE_OFFSET;
    m_openAngle = 0.0f;
    m_initialPos = POS;

    /** 物理静的オブジェクトをモデルから生成*/
    physicsStaticObject.CreateFromModel(m_hitBox.GetModel(), m_hitBox.GetModel().GetWorldMatrix());
    m_physicsState.CreateFromModel(m_blockState.GetModel(), m_blockState.GetModel().GetWorldMatrix());
    m_physicsBoss.CreateFromModel(m_brockBoss.GetModel(), m_brockBoss.GetModel().GetWorldMatrix());

    m_physicsBoss.SetPosition(POS_HIDE_STATE);

    /** 物理静的オブジェクトをモデルから生成*/
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
        m_physicsState.SetPosition(Vector3(0.0f, -2000.0f, 0.0f));
        // 指定の角度になるまで開き続ける
        if (m_openAngle > DOOR_TARGET_ANGLE)
        {
            // 経過時間分だけ角度を足す
            m_openAngle += DOOR_OPEN_SPEED * g_gameTime->GetFrameDeltaTime();

            // 行き過ぎないように-51度で止める
            if (m_openAngle < DOOR_TARGET_ANGLE) {
                m_openAngle = DOOR_TARGET_ANGLE;
            }

            // 回転クォータニオンを作成
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

    Boss* boss = nullptr;
    if (EnemyManager::GetInstance() != nullptr) {
        boss = EnemyManager::GetInstance()->GetBoss();
    }

    // ボスが存在して、かつ生きているなら
    if (boss != nullptr && !boss->GetStatus()->IsDead()) {
        // 壁を元の位置（地上）に出現させる！
        m_physicsBoss.SetPosition(Vector3::Zero);
    }
    else {
        m_physicsBoss.SetPosition(POS_HIDE_BOSS);
    }

    // 各モデルの更新
    m_map.Update();
    m_anim.Update();
    m_pro.Update();
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
    m_pro.Draw(rc);
}