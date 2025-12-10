#include "stdafx.h"
#include "BackGround.h"
#include"Game.h"

/**
 * @brief BackGroundクラスのコンストラクタ
 * @details 背景モデルの初期化、位置・回転・スケール設定、物理オブジェクト生成、デバッグワイヤーフレーム表示を行います。
 */
BackGround::BackGround()
{
    /** モデルの位置を設定*/
    m_map.SetPosition(0.0f, 0.0f, 0.0f);
    /** 背景モデルを初期化*/
    m_map.Init("Assets/modelData/Stage/map.tkm");


    m_anim.SetPosition(0.0f, 0.0f, 0.0f);
    m_anim.Init("Assets/modelData/Stage/reizouko_anim.tkm");


    m_hingeOffset = Vector3(250.0f, 0.0f, 220.0f);


    m_openAngle = 0.0f;

    m_hitBox.SetPosition(0.0f, 0.0f, 0.0f);
    m_hitBox.Init("Assets/modelData/Stage/map_HitBox.tkm");
    m_hitBox.Update();
    /** 物理静的オブジェクトをモデルから生成*/
    physicsStaticObject.CreateFromModel(m_hitBox.GetModel(), m_hitBox.GetModel().GetWorldMatrix());


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

void BackGround::Update()
{
    if (Game::IsGamePlay)
    {
        if(m_openAngle >-51.0f)
        {
            // 開くスピード (1秒間に100度動く)
            float openSpeed = -25.0f;
            
            
            // 経過時間分だけ角度を足す
            m_openAngle+=openSpeed * g_gameTime->GetFrameDeltaTime();


            //行き過ぎないように-50度で止める
            if (m_openAngle < -51.0f) {
                m_openAngle = -51.0f;
            }


            //回転を作成 (Y軸を中心に回転)
            Quaternion rot;
            rot.SetRotationDeg(Vector3::AxisY, m_openAngle);
            m_anim.SetRotation(rot);


            // ヒンジのオフセットを、今の角度で回転させる
            Vector3 currentOffset = m_hingeOffset;
            rot.Apply(currentOffset);


            // 「回す前のオフセット」と「回した後のオフセット」の差分を計算
            Vector3 positionCorrection = m_hingeOffset - currentOffset;


            // 初期位置に補正値を足してセット
            m_anim.SetPosition(m_initialPos + positionCorrection);
        }
    }
    m_map.Update();
    m_anim.Update();
}
/**
 * @brief 背景の描画処理
 * @param rc 描画コンテキスト
 * @details 背景モデルの描画を行います。
 */
void BackGround::Render(RenderContext& rc)
{
    m_map.Draw(rc);
    m_anim.Draw(rc);
}