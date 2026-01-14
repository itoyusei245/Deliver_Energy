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
	AthleticBase() {}
	virtual ~AthleticBase() {}
	virtual void Render(RenderContext& rc) override {
			m_modelRender.Draw(rc);
	}

	CollisionObject* GetCollisionObject() { return &m_collisionObject; }

protected:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	CollisionObject m_collisionObject;
};