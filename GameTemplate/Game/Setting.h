#pragma once
class Setting : public IGameObject
{
public:
	Setting();
	~Setting();


	void Update();


	void Render(RenderContext& rc);


	SpriteRender m_settingSprite;
	SpriteRender m_picUpSprite[4];
	SpriteRender m_barMaster;
	SpriteRender m_barBGM;
	SpriteRender m_barSE;
	SpriteRender m_barMasterFlame;
	SpriteRender m_barBGMFlame;
	SpriteRender m_barSEFlame;

	
private:
	int m_currentBar = 0;

	int m_state = 0;

	/** 表示中かどうかのフラグ */
	bool m_isActive = false;

	void UpdatePicUp();
	
	//音量操作用の関数
	void UpdateVolumeControl();
};

