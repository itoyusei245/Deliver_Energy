#pragma once
#include"sound/SoundSource.h"
class Player : public IGameObject
{
public:
	Player();//�R���X�g���N�^�@NewGO�ō��ꂽ�Ƃ��ɏo�����{���͂����ƕ��G
    ~Player();//�f�X�g���N�^�@DeleteGO�����ꂽ�Ƃ��ɏo��
    //�X�V����
	void Update();
    //�`�揈��
	void Render(RenderContext& rc);
    //�ړ�����
	void Move();
    //��]����
    void Rotation();
    // �X�e�[�g�Ǘ�
    void ManageState();
    //�A�j���[�V�����̍Đ�
    void Animation();

    //�����o�ϐ�
	ModelRender modelRender;//���f�������_�\
	Vector3 position;       //���W
    //�A�j���[�V����
    enum EnAnimationClip { 
        enAnimationClip_Idle,
        enAnimationClip_Walk,
        enAnimationClip_Jump,
        enAnimationClip_Num,
    };
    AnimationClip animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v
    CharacterController characterController; //�L�����N�^�[�R���g���[���[
    Vector3 moveSpeed; //�ړ����x
    Quaternion rotation;//�N�H�[�^�j�I��
    int playerState = 0;//�v���C���[�̃X�e�[�g(���)��\���ϐ�
    int itemCount = 0;//�W�߂��R�C���̐����J�E���g
    int jump=0;
    int jumpState = 0;
    SoundSource* m_sound;
};

