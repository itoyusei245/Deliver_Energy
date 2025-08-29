#include "stdafx.h"
#include "Player.h"
#include"sound/SoundEngine.h"

Player::Player()
{
	//���j�e�B�����̃��f����ǂݍ���
	modelRender.Init("Assets/modelData/Player.tkm", animationClips,enAnimationClip_Num, enModelUpAxisY);
	//modelRender.Update();

	//�L�����R����������
	characterController.Init(10.0f, 10.0f, position);
	position = { -20.0f, -600.0f, 0.0f };//�����ʒu

}

Player::~Player()
{
	DeleteGO(m_sound);
}

//�X�V����
void Player::Update()
{
	//�ړ������B
	Move();

	//��]�����B
	Rotation();

	//�X�e�[�g�Ǘ��B
	ManageState();

	//�A�j���[�V�����̍Đ��B
	Animation();

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
			g_soundEngine->ResistWaveFileBank(0, "Assets/Sound/jump.wav");
			m_sound = NewGO<SoundSource>(0);
			m_sound->Init(0);
			m_sound->Play(false);

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
	else if (jump == 0) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			moveSpeed.y += 300.0f;
			jump = 1;
			playerState = 1;
		}
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

//�X�e�[�g�Ǘ�
void Player::ManageState()
{
	//�n�ʂɕt���Ă��Ȃ�������B
	if (characterController.IsOnGround() == false)
	{
		//�X�e�[�g��1(�W�����v��)�ɂ���B
		playerState = 1;
		//������ManageState�̏������I��点��B
		return;
	}

	//�n�ʂɕt���Ă�����B
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//�X�e�[�g��2(����)�ɂ���B
		playerState = 2;
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��0(�ҋ@)�ɂ���B
		playerState = 0;
	}
}

void Player::Animation()
{
	//switch���B
	switch (playerState) {
		//�v���C���[�X�e�[�g��0(�ҋ@)��������B
	case 0:
		//�ҋ@�A�j���[�V�������Đ�����B
		modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//�v���C���[�X�e�[�g��1(�W�����v��)��������B
	case 1:
		//�W�����v�A�j���[�V�������Đ�����B
		modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//�v���C���[�X�e�[�g��2(����)��������B
	case 2:
		//�����A�j���[�V�������Đ�����B
		modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}