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
            // �|���
            targetRotation.SetRotationDeg(Vector3::AxisX, 90.0f);
            isFallen = true;
            rollAngle = 0.0f; // �ݐω�]���Z�b�g
        }
        else {
            // �N���オ��
            targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
            isFallen = false;
            rollAngle = 0.0f;
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
            // �|��Ă���Ƃ��F���͕����x�N�g�������ɃS���S����]
            // ================================
            Quaternion dirRot;
            dirRot.SetRotationYFromDirectionXZ(inputDir);

            Quaternion fallenBase;
            fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

            // ���̃t���[���̈ړ�����
            float distance = moveSpeed.Length() * (1.0f / 60.0f);

            // �ݐω�]�p�x���X�V�i�֒����邽�߂ɔ{����������j
            rollAngle += (distance / 25.0f) * 2.5f;

            // ���͕��������̂܂܉�]���ɗ��p
            Vector3 rollAxis = inputDir;
            rollAxis.Normalize();

            Quaternion rollRot;
            rollRot.SetRotation(rollAxis, rollAngle);

            Quaternion tmp;
            tmp.Multiply(fallenBase, dirRot);
            targetRotation.Multiply(tmp, rollRot);
        }
        else {
            // ================================
            // �������F���͕�����O�ɂ���
            // ================================
            targetRotation.SetRotationYFromDirectionXZ(inputDir);
        }
    }

    // ��]���
    rotation.Slerp(0.2f, rotation, targetRotation);

    // ====== �`��p�̈ʒu�␳ ======
    Vector3 renderPos = position;

    if (!isFallen) {
        // �������[�h �� pivot �����S�Ȃ̂ŁA���a�Ԃ񕂂����Ē��ڒn
        renderPos.y += 40.0f;
    }
    else {
        // �|�ꃂ�[�h �� pivot �����S�Ȃ̂ŁA���������Ԃ񕂂����đ��ʂ�ڒn
        renderPos.y += 25.0f;
    }

    // ���f���ɔ��f
    modelRender.SetPosition(renderPos);
    modelRender.SetRotation(rotation);
}



void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}