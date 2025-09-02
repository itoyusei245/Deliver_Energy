#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	//���j�e�B�����̃��f����ǂݍ���
	modelRender.Init("Assets/animData/Player.tkm");

	//�L�����R����������
	characterController.Init(25.0f, 50.0f, position);
	position = { 10.0f, 10.0f, 0.0f };//�����ʒu
	rotation.SetRotationDeg(Vector3::AxisX, 0.0f);
	targetRotation = rotation;

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
	// B�{�^���œ|���^�N���オ���؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonB)) {
		if (!isFallen) {
			// �|���p����ڕW��
			targetRotation.SetRotationDeg(Vector3::AxisX, 90.0f);
			isFallen = true;
		}
		else {
			// �����p����ڕW��
			targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
			isFallen = false;
		}
	}

	// �X�e�B�b�N���͕������Z�o
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.z = g_pad[0]->GetLStickYF();
	Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

	if (inputDir.LengthSq() > 0.01f) {
		inputDir.Normalize();

		if (isFallen) {
			// ================================
			// �|��Ă���Ƃ��F���|�� + ���͕�����O�ɂ���
			// ================================
			Quaternion dirRot;
			dirRot.SetRotationYFromDirectionXZ(inputDir);

			Quaternion fallenBase;
			fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

			targetRotation.Multiply(fallenBase, dirRot);
		}
		else {
			// ================================
			// �������F���͕�����O�ɂ���
			// ================================
			targetRotation.SetRotationYFromDirectionXZ(inputDir);
		}
	}

	// ��Ԃ��ĉ�]
	rotation.Slerp(0.2f, rotation, targetRotation);

	// ���f���ɔ��f
	modelRender.SetRotation(rotation);
}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}