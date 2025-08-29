#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include"Image.h"
Item::Item()
{
	//アイテムのモデルを読み込む。
	modelRender.Init("Assets/modelData/map/coin.tkm");
	modelRender.Update();
	//キャラコンを初期化
	player = FindGO<Player>("player");
	position = { -20.0f, 300.0f, 0.0f };//初期位置
	//サウンドのロード
	g_soundEngine->ResistWaveFileBank(1, "Assets/Sound/coin.wav");
	m_coinSound = NewGO<SoundSource>(0);
	m_coinSound->Init(1);
	
}
Item::~Item()
{

}
//アイテムのカウントが１増えたら画像を読み込む。
void Item::Update()
{
	//移動処理
	Move();

	//回転処理
	Rotation();

	//絵描きさんの更新処理
	modelRender.Update();

	Vector3 diff = player->position - position;
	//ベクトルの長さが20.0fより小さかったら
	if (diff.Length() <= 20.0f)
	{
			player->itemCount += 1;
			m_coinSound->Play(false);

			//自身を削除する。
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
//プレイヤーがブロックに接したらコインを出現させる
//コインを取ったら説明の画像を拡大表示させる
void Item::Move()
{
	//position.z += 0.5f;
	modelRender.SetPosition(position);
}

void Item::Rotation()
{
	//回転を加算する。
	rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	modelRender.SetRotation(rotation);
}


void Item::Render(RenderContext& rc)
{
	//アイテムを描画する。
	modelRender.Draw(rc);
}
