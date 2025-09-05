#pragma once
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();

	void Update();

	void Render(RenderContext&rc);
	
	void Move();

	void Rotation();

	ModelRender			modelRender;
	Vector3				position;
	Vector3				moveSpeed;
	CharacterController characterController;
	Quaternion			rotation;
};

