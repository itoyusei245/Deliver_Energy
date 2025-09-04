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
    // ���͂���XZ�ړ������
    moveSpeed.x = 0.0f;
    moveSpeed.z = 0.0f;

    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    Vector3 forward = g_camera3D->GetForward();
    Vector3 right = g_camera3D->GetRight();
    forward.y = 0.0f;
    right.y = 0.0f;

    right *= stickL.x * 180.0f;
    forward *= stickL.y * 180.0f;


    Vector3 inputMove = right + forward;

    if (isFallen) {
        // ================================
        // �|��Ă���Ƃ� �� �]�������������
        // ================================
        if (inputMove.LengthSq() > 0.01f) {
            inputMove.Normalize();
            // ���݂̑��x�ɏ���������
            rollVelocity += inputMove * 10.0f;   // 10.0f �������̋���
            // �ő�X�s�[�h����
            if (rollVelocity.Length() > 600.0f) {
                rollVelocity.Normalize();
                rollVelocity *= 600.0f;
            }
        }
        else {
            // ���͂��Ȃ���Ώ��X�Ɍ���
            rollVelocity *= 0.5f;
        }

        moveSpeed += rollVelocity;
    }
    else {
        // ������ �� ���͂����������ړ�
        moveSpeed += inputMove;
        // �|�����͓]���葬�x�����Z�b�g
        rollVelocity = { 0,0,0 };
    }

    // ====== �W�����v���� ======
    if (characterController.IsOnGround()) {
        moveSpeed.y = 0.0f;
        if (g_pad[0]->IsTrigger(enButtonA)) {
            if (isFallen) {
                // �]���葬�x������ƃW�����v����
                float boost = rollVelocity.Length() * 0.4f;
                moveSpeed.y = 300.0f + boost;
            }
            else {
                moveSpeed.y = 400.0f;
            }
        }
    }
    else {
        moveSpeed.y -= 8.5f; // �d��
    }

    // ���ۂɈړ�
    position = characterController.Execute(moveSpeed, 1.0f / 60.0f);
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
            rollAngle = 0.0f;// �ݐω�]���Z�b�g
        }
        else {
            // �N���オ��
            targetRotation.SetRotationDeg(Vector3::AxisX, 0.0f);
            isFallen = false;
			rollAngle = 0.0f;// �ݐω�]���Z�b�g
        }
    }

    // �X�e�B�b�N���͕������Z�o
    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.z = g_pad[0]->GetLStickYF();
    Vector3 inputDir = { stickL.x, 0.0f, stickL.z };

	if (inputDir.LengthSq() > 0.01f) {// ���͂�����Ƃ��̂݉�]����
		inputDir.Normalize();// ���K��

        if (isFallen) {
            // ================================
            // �|��Ă���Ƃ��F���͕����x�N�g�������ɃS���S����]
            // ================================
			
			// ���͕�����������]
			Quaternion dirRot;

			// Y����]�݂̂Ō��������킹��
			dirRot.SetRotationYFromDirectionXZ(inputDir);

			// �|�ꂽ�Ƃ��̊�{��]
			Quaternion fallenBase;
			
			// X��90�x�|��
			fallenBase.SetRotationDeg(Vector3::AxisX, 90.0f);

            // ���̃t���[���̈ړ�����
            float distance = moveSpeed.Length() * (1.0f / 60.0f);

            // �ݐω�]�p�x���X�V�i�֒����邽�߂ɔ{����������j
            rollAngle += (distance / 25.0f) * 1.0f;

            // ���͕��������̂܂܉�]���ɗ��p
            Vector3 rollAxis = inputDir;
            rollAxis.Normalize();

			// �]�����]
			Quaternion rollRot;

			// ���Ɗp�x�ŉ�]���Z�b�g
            rollRot.SetRotation(rollAxis, rollAngle);

			// �ŏI�I�ȖڕW��]���v�Z
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