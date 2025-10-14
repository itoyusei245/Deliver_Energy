#include "stdafx.h"
#include "BackGround.h"

/**
 * @brief BackGroundクラスのコンストラクタ
 * @details 背景モデルの初期化、位置・回転・スケール設定、物理オブジェクト生成、デバッグワイヤーフレーム表示を行います。
 */
BackGround::BackGround()
{
	/** 背景モデルを初期化*/
    modelRender.Init("Assets/modelData/Stage/map.tkm");
    //modelRender.Init("Assets/modelData/Stage/BackGround.tkm");
    /** モデルの位置を設定*/
    modelRender.SetPosition(0.0f, 0.0f, 0.0f);
    /** Y軸回転を90度に設定*/
    m_rot.SetRotationDegY(90);
    /** モデルの回転を設定*/
    modelRender.SetRotation(m_rot);
    /** モデルのスケールを設定*/
    modelRender.SetScale(Vector3(1.3f, 1.3f, 1.3f));
    /** モデルの更新*/
    modelRender.Update();
    /** 物理静的オブジェクトをモデルから生成*/
    physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
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
 * @brief 背景の描画処理
 * @param rc 描画コンテキスト
 * @details 背景モデルの描画を行います。
 */
void BackGround::Render(RenderContext& rc)
{
    modelRender.Draw(rc);
}