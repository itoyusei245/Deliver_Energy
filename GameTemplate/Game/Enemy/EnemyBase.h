/**
 * EnemyBase.h
 * 敵キャラクターを作るときに継承してほしい基底クラス
 */
#pragma once
#include "EnemyStatus.h"


/**
 * 敵キャラクターの基底クラス
 * NOTE: 敵キャラクターを作るときは継承してください
 */
class EnemyBase : public IGameObject
{
public:
	EnemyBase() {}
	virtual ~EnemyBase() {}

	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}

	void SetPosition(const Vector3& position) { m_position = position; }
	const Vector3& GetPosition() const { return m_position; }

	void SetDefaultPosition(const Vector3& position) { m_defaultPosition = position; }
	const Vector3& GetDefaultPosition() const { return m_defaultPosition; }

	void SetScale(const Vector3& scale) { m_scale = scale; }
	const Vector3& GetScale() const { return  m_scale; }

	void SetRotation(const Quaternion& rotation) { m_rotation = rotation; }
	const Quaternion& GetRotation() const { return m_rotation; }

	CollisionObject* GetCollisionObject() { return &m_collisionObject; }


protected:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Vector3 m_defaultPosition = Vector3::Zero;

	Quaternion m_rotation = Quaternion::Identity;

	ModelRender m_modelRender;

	CollisionObject m_collisionObject;

	IEnemyStatus* m_status = nullptr;

	PhysicsStaticObject m_physicsStaticObject;
};