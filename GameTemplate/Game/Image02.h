#pragma once
class Game;
class Image02 :public IGameObject
{
public:
	Image02();
	~Image02();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);
	Game* game;
	//メンバ変数
	SpriteRender spriteRender; //スプライトレンダ―
};
