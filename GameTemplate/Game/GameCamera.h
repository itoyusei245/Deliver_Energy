#pragma once

class Player;
//�Q�[�����̃J�����𐧌䂷��
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	Player* player;//�v���C���[
	Vector3 m_toCameraPos;//�����_���王�_�Ɍ������x�N�g��
};

