#include "stdafx.h"
#include "BackGround.h"

/**
 * @brief BackGroundクラスのコンストラクタ
 * @details 背景モデルの初期化、位置・回転・スケール設定、物理オブジェクト生成、デバッグワイヤーフレーム表示を行います。
 */
BackGround::BackGround()
{
	/** 背景モデルを初期化*/
    m_map.Init("Assets/modelData/Stage/map.tkm");
    //modelRender.Init("Assets/modelData/Stage/BackGround.tkm");
    /** モデルの位置を設定*/
    m_map.SetPosition(0.0f, 0.0f, 0.0f);
    /** モデルの回転を設定*/
    m_map.SetRotation(m_rot);
    /** モデルのスケールを設定*/
    m_map.SetScale(Vector3(1.3f, 1.3f, 1.3f));
    /** モデルの更新*/
    m_map.Update();
    /** Y軸回転を90度に設定*/
    m_rot.SetRotationDegY(90);
    /** 物理静的オブジェクトをモデルから生成*/
    physicsStaticObject.CreateFromModel(m_map.GetModel(), m_map.GetModel().GetWorldMatrix());
    /** デバッグ用ワイヤーフレーム表示を有効化*/
    /**PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();*/
}

/**
 * @brief BackGroundクラスのデストラクタ
 */
BackGround::~BackGround()
{

}

/**
 * @brief 背景の描画処理
 * @param rc 描画コンテキスト
 * @details 背景モデルの描画を行います。
 */
void BackGround::Render(RenderContext& rc)
{
    m_map.Draw(rc);
}