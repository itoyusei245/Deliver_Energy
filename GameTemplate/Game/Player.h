#pragma once
class Player : public IGameObject
{
public:
	Player();//コンストラクタ　NewGOで作られたときに出現※本当はもっと複雑
    ~Player();//デストラクタ　DeleteGO消されたときに出現
    //更新処理
	void Update();
    //描画処理
	void Render(RenderContext& rc);
    //移動処理
	void Move();
    //回転処理
    void Rotation();

    //メンバ変数
	ModelRender modelRender;//モデルレンダ―
	Vector3 position;       //座標
    //アニメーション
    
    CharacterController characterController; //キャラクターコントローラー
    Vector3 moveSpeed; //移動速度
    Quaternion rotation;//クォータニオン
    int playerState = 0;//プレイヤーのステート(状態)を表す変数
    int itemCount = 0;//集めたコインの数をカウント
    int jump=0;
    int jumpState = 0;
};

