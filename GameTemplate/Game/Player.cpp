#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//���j�e�B�����̃��f����ǂݍ���
	modelRender.Init("Assets/animData/Player.tkm");

	//�L�����R����������
	characterController.Init(10.0f, 10.0f, position);
	position = { 10.0f, 10.0f, 0.0f };//�����ʒu

}

Player::~Player()
{
}

//�X�V����
void Player::Update()
{
	//�ړ������B
	Move();

	//��]�����B
	Rotation();

	//�G�`������̍X�V�����B
	modelRender.Update();

	//m_sound->SetVolume(3.5f);

}

void Player::Move()
{
	//xz�̈ړ����x��0.0f�ɂ���B
	moveSpeed.x =0.0f;
	moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 125.0f;
	forward *= stickL.y * 125.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	moveSpeed += right + forward;
	if (jumpState == 0)
	{

	}
	//�n�ʂɕt���Ă�����B
	if (characterController.IsOnGround())
	{
		//�d�͂𖳂����B
		moveSpeed.y = 0.0f;

		//A�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�W�����v������B
			moveSpeed.y = 300.0f;
			jump += 1;

		}
	}
	//�n�ʂɕt���Ă��Ȃ�������B
	else
	{
		//�d�͂𔭐�������B
		moveSpeed.y -= 7.0f;
		//m_sound->Stop();
	}

	if (characterController.IsOnGround() == true) {
		playerState = 0;

		jump = 0;
	}
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	//�G�`������ɍ��W��������B
	modelRender.SetPosition(position);
}

void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		rotation.SetRotationYFromDirectionXZ(moveSpeed);

		//�G�`������ɉ�]��������B
		modelRender.SetRotation(rotation);

	}

}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}