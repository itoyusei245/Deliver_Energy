#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include"Image.h"
Item::Item()
{
	//�A�C�e���̃��f����ǂݍ��ށB
	modelRender.Init("Assets/modelData/map/coin.tkm");
	modelRender.Update();
	//�L�����R����������
	player = FindGO<Player>("player");
	position = { -20.0f, 300.0f, 0.0f };//�����ʒu
	//�T�E���h�̃��[�h
	g_soundEngine->ResistWaveFileBank(1, "Assets/Sound/coin.wav");
	m_coinSound = NewGO<SoundSource>(0);
	m_coinSound->Init(1);
	
}
Item::~Item()
{

}
//�A�C�e���̃J�E���g���P��������摜��ǂݍ��ށB
void Item::Update()
{
	//�ړ�����
	Move();

	//��]����
	Rotation();

	//�G�`������̍X�V����
	modelRender.Update();

	Vector3 diff = player->position - position;
	//�x�N�g���̒�����20.0f��菬����������
	if (diff.Length() <= 20.0f)
	{
			player->itemCount += 1;
			m_coinSound->Play(false);

			//���g���폜����B
			DeleteGO(this);

	}

	

	if (m_coinState == true)
	{
		if (m_coinSound->IsPlaying() == false)
		{
			DeleteGO(this);
		}
	}
		

	

}
//�v���C���[���u���b�N�ɐڂ�����R�C�����o��������
//�R�C���������������̉摜���g��\��������
void Item::Move()
{
	//position.z += 0.5f;
	modelRender.SetPosition(position);
}

void Item::Rotation()
{
	//��]�����Z����B
	rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	modelRender.SetRotation(rotation);
}


void Item::Render(RenderContext& rc)
{
	//�A�C�e����`�悷��B
	modelRender.Draw(rc);
}
