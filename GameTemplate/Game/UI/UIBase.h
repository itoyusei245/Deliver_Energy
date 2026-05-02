/**
 * UIBase.h
 * UIの基本的な処理をするクラス群
 */
#pragma once
#include "stdafx.h"
#include "UIAnimation.h"

struct UITransform {
	Vector3 m_localPosition = { 0.0f, 0.0f, 0.0f };
	Vector3 m_localScale = { 1.0f, 1.0f, 1.0f };
	Quaternion m_localRotation;

	Vector3 m_position = { 0.0f, 0.0f, 0.0f };
	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
	Quaternion m_rotation;

	UITransform* m_parent = nullptr;

	// 親子関係のセット
	void SetParent(UITransform* parent) {
		m_parent = parent;
	}

	// 親子関係の解除
	void RemoveChild(UITransform* child) {
		if (child != nullptr && child->m_parent == this) {
			child->m_parent = nullptr;
		}
	}

	// 座標の計算
	void UpdateTransform() {
		if (m_parent) {
			m_position = m_parent->m_position + m_localPosition;
			m_scale.x = m_parent->m_scale.x * m_localScale.x;
			m_scale.y = m_parent->m_scale.y * m_localScale.y;
			m_scale.z = m_parent->m_scale.z * m_localScale.z;
			m_rotation = m_localRotation; 
		}
		else {
			m_position = m_localPosition;
			m_scale = m_localScale;
			m_rotation = m_localRotation;
		}
	}
};

class UIBase : public Noncopyable
{
public:
	UITransform m_transform;
	Vector4 color_ = Vector4::White; //!< カラー

	std::vector<std::unique_ptr<UIAnimationBase>> uiAnimationList_; //!< UIアニメーションリスト

	bool isStart = false;	//!< スタートフラグ
	bool isUpdate = true;	//!< 更新フラグ
	bool isDraw = true;		//!< 描画フラグ

public:
	UIBase()
	{
	}
	virtual ~UIBase()
	{
	}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

public:
	/**
	 * @brief 表示・非表示を切り替える
	 * @param flag trueで表示、falseで非表示
	 */
	void SetEnable(bool flag) {
		isDraw = flag;
	}

	void UpdateAnimation()
	{
		for (auto& ui : uiAnimationList_) {
			ui.get()->Update();
		}
	}
	void PlayAnimation()
	{
		for (auto& ui : uiAnimationList_) {
			ui.get()->Play();
		}
	}
	bool IsPlayAnimation() const
	{
		for (auto& ui : uiAnimationList_) {
			if (ui.get()->IsPlay()) return true;
		}
		return false;
	}

	void SetUIAnimation(std::unique_ptr<UIAnimationBase>animation);
};


// ============================================
// 画像を使うUI関連
// ============================================

class UIImage : public UIBase
{
protected:
	SpriteRender m_spriteRender;

public:
	UIImage();
	~UIImage();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	/** スプライトレンダーの取得 */
	SpriteRender* GetSpriteRender() { return &m_spriteRender; }
};

/**
 * ゲージUI
 */
class UIGauge : public UIImage
{
	friend class UICanvas;

private:
	UIGauge();
	~UIGauge();

public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	void Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation);

	void CircleInitialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation);

	void SetProgress(float progress);

private:
	bool m_isCircle = false;
	float m_circleProgress = 1.0f;
	float m_circleSize = 1.0f;
};

/**
 * アイコンUI
 */
class UIIcon : public UIImage
{
	friend class UICanvas;

public:
	UIIcon();
	~UIIcon();

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	void Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation);
};


// ============================================
// 文字を使うUI関連
// ============================================

class UIText : public UIBase
{
protected:
	FontRender m_fontRender;

private:
	UIText();
	~UIText();

public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
};

class UINumber : public UIBase
{
	friend class UICanvas;
private:
	UINumber();
	~UINumber();

public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

public:
	void Initialize(const char* dirPath, const char* ext, const float digitWidth, const float digitHeight, const Vector3& position, float digitSpacing = 50.0f);

	/**
	 * @brief 表示する数字をセットする
	 */
	void SetNumber(int number);

	/**
	 * @brief ゼロ埋め表示の切り替え
	 */
	void SetZeroPadding(bool isPad, int padLength = 2) {
		m_isZeroPadding = isPad;
		m_padLength = padLength;
	}

private:
	std::vector<std::unique_ptr<SpriteRender>> m_digitSprites;

	int m_currentNumber = -1;
	float m_digitWidth = 50.0f;
	float m_digitHeight = 50.0f;
	float m_digitSpacing = 50.0f;

	std::string m_dirPath;
	std::string m_ext;

	bool m_isZeroPadding = false;
	int m_padLength = 2;
};


// ============================================
// キャンバス
// ============================================

/**
 * 絵を書くキャンバスのイメージ
 * UIを作るときにこのクラスを作る
 */
class UICanvas : public UIBase
{
	friend class UIBase;
	friend class UIImage;
	friend class UIGauge;
	friend class UIIcon;
	friend class UIText;
	friend class UINumber;
	friend class UIButton;

private:
	std::vector<UIBase*> m_uiList;

public:
	UICanvas();
	~UICanvas();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	template <typename T>
	T* CreateUI()
	{
		T* ui = new T();
		ui->m_transform.SetParent(&m_transform);
		m_uiList.push_back(ui);
		return ui;
	}
};