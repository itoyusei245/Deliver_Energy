#pragma once
class Player : public IGameObject
{
public:
    Player();//�R���X�g���N�^�@NewGO�ō��ꂽ�Ƃ��ɏo��
    ~Player();//�f�X�g���N�^�@DeleteGO�����ꂽ�Ƃ��ɏo��
    //�X�V����
    void Update();
    //�`�揈��
    void Render(RenderContext& rc);
    //�ړ�����
    void Move();
    //��]����
    void Rotation();

    //�����o�ϐ�
    ModelRender modelRender;//���f�������_�\
    Vector3 position;       //���W
    //�A�j���[�V����

    Vector3 rollVelocity = { 0.0f, 0.0f, 0.0f };  // �|��Ă���Ƃ��̓]���葬�x
    CharacterController characterController; //�L�����N�^�[�R���g���[���[
    Vector3 moveSpeed; //�ړ����x
    Quaternion rotation;//�N�H�[�^�j�I��
    int playerState = 0;//�v���C���[�̃X�e�[�g(���)��\���ϐ�
    int jump = 0;
    int jumpState = 0;
    bool isFallen = false;  // �|��Ă��邩�ǂ���
    Quaternion targetRotation;   // �ڕW�̉�]
};

