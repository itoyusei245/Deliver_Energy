/**
 * AthleticBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once
/**
 * Athleticキャラクターの基底クラス
 * NOTE: Athleticキャラクターを作るときは継承してください
 */
class AthleticBase : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	 */
	AthleticBase() {}


	/**
	 * @brief デストラクタ
	 */
	virtual ~AthleticBase() {}
	
	
	/**
	 * @brief 描画処理を行う
	 * @details 保持しているモデルの描画処理を実行します。
	 * @param[in,out] rc 描画コンテキスト（レンダリング設定やステートを持つクラス）
	 */
	virtual void Render(RenderContext& rc) override {
			m_modelRender.Draw(rc);
	}


	/**
	 * @brief コリジョンオブジェクトを取得する
	 * @return CollisionObject* コリジョンオブジェクトへのポインタ
	 */
	CollisionObject* GetCollisionObject() { return &m_collisionObject; }

protected:
	/**
	 * @brief モデル描画用オブジェクト
	 */
	ModelRender m_modelRender;
	
	
	/**
	 * @brief 静的物理オブジェクト
	 * @note 地形や動かない障害物として判定を持たせるために使用します。
	 */
	PhysicsStaticObject m_physicsStaticObject;


	/**
	 * @brief 衝突判定用オブジェクト
	 */
	CollisionObject m_collisionObject;
};