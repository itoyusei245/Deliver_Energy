#pragma once
class Game;
class Image03 :public IGameObject
{
public:
	Image03();
	~Image03();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);
	Game* game;
	//メンバ変数
	SpriteRender spriteRender; //スプライトレンダ―
};

