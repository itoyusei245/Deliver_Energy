#pragma once
#include"sound/SoundSource.h"
class Player;
//アイテムクラス
class Item:public IGameObject
{
public:
	Item();
	~Item();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);
	//移動処理
	void Move();
	//回転処理
	void Rotation();

	//メンバ変数
	/*ItemCount itemCount;*/
	SpriteRender spriteRender;
	ModelRender modelRender;	//モデルレンダ―
	Vector3 position;		//座標
	int moveState=0;		//上下移動
	Vector3 firstPosition;		//初期座標
	Quaternion rotation;	//クォータニオン
	Player* player;		//プレイヤー
	Vector3 moveSpeed; //移動速度
	//CharacterController characterController; //キャラクターコントローラー
	SoundSource* m_coinSound;
	SoundSource* se;
	//bool型の変数は true か false の2つしか入らない
	bool m_coinState = false;
	bool m_fast = true;
};


