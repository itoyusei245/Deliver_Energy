#pragma once
#include"sound/SoundSource.h"
class Player;
//�A�C�e���N���X
class Item:public IGameObject
{
public:
	Item();
	~Item();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);
	//�ړ�����
	void Move();
	//��]����
	void Rotation();

	//�����o�ϐ�
	/*ItemCount itemCount;*/
	SpriteRender spriteRender;
	ModelRender modelRender;	//���f�������_�\
	Vector3 position;		//���W
	int moveState=0;		//�㉺�ړ�
	Vector3 firstPosition;		//�������W
	Quaternion rotation;	//�N�H�[�^�j�I��
	Player* player;		//�v���C���[
	Vector3 moveSpeed; //�ړ����x
	//CharacterController characterController; //�L�����N�^�[�R���g���[���[
	SoundSource* m_coinSound;
	SoundSource* se;
	//bool�^�̕ϐ��� true �� false ��2��������Ȃ�
	bool m_coinState = false;
	bool m_fast = true;
};


