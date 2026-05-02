/**
 * SpriteAnimation.h
 * SpriteRenderをアニメーションさせる機能群
 */
#pragma once
#include <functional>

class UIBase;

template <typename T>
using UIAnimationApplyFunc = std::function<void(const T&)>;

/** アニメーションの再生モード */
enum class UILoopMode {
	Once,       // 1回だけ再生して止まる
	Loop,       // 繰り返す
	PingPong    // 行って帰ってを繰り返す
};

/**
 * @brief アニメーション基底クラス
 */
class UIAnimationBase
{
protected:
	UIBase* m_ui = nullptr;

	bool m_isPlaying = false;
	float m_currentTime = 0.0f;
	float m_duration = 0.0f;
	UILoopMode m_loopMode = UILoopMode::Once;
	bool m_isReverse = false;

public:
	UIAnimationBase() {}
	virtual ~UIAnimationBase() {}

	virtual void Update() = 0;
	virtual void Play() = 0;
	bool IsPlay() { return m_isPlaying; }

	void SetUI(UIBase* ui) { m_ui = ui; }
	void Stop() { m_isPlaying = false; }
};

//=======================================================
//   Vector2用アニメーションクラス
//=======================================================
class UIVector2Animation : public UIAnimationBase
{
protected:
	Vector2 m_start = Vector2::Zero;
	Vector2 m_end = Vector2::Zero;
	Vector2 m_currentValue = Vector2::Zero;
	UIAnimationApplyFunc<Vector2> m_applyFunc;

public:
	UIVector2Animation() {}
	virtual ~UIVector2Animation() {}

	void SetParameter(const Vector2& start, const Vector2& end, float duration, UILoopMode loopMode = UILoopMode::Once) {
		m_start = start;
		m_end = end;
		m_duration = duration;
		m_loopMode = loopMode;
	}

	void SetFunc(const UIAnimationApplyFunc<Vector2>& func) { m_applyFunc = func; }

	void Play() override;
	void Update() override;

	Vector2 GetCurrentValue() const { return m_currentValue; }
};

//=======================================================
//   Vector3用アニメーションクラス
//=======================================================
class UIVector3Animation : public UIAnimationBase
{
protected:
	Vector3 m_start = Vector3::Zero;
	Vector3 m_end = Vector3::Zero;
	Vector3 m_currentValue = Vector3::Zero;
	UIAnimationApplyFunc<Vector3> m_applyFunc;

public:
	UIVector3Animation() {}
	virtual ~UIVector3Animation() {}

	void SetParameter(const Vector3& start, const Vector3& end, float duration, UILoopMode loopMode = UILoopMode::Once) {
		m_start = start;
		m_end = end;
		m_duration = duration;
		m_loopMode = loopMode;
	}

	void SetFunc(const UIAnimationApplyFunc<Vector3>& func) { m_applyFunc = func; }

	void Play() override;
	void Update() override;

	Vector3 GetCurrentValue() const { return m_currentValue; }
};

//=======================================================
//   Vector4用アニメーションクラス
//=======================================================
class UIVector4Animation : public UIAnimationBase
{
protected:
	Vector4 m_start = Vector4::White;
	Vector4 m_end = Vector4::White;
	Vector4 m_currentValue = Vector4::White;
	UIAnimationApplyFunc<Vector4> m_applyFunc;

public:
	UIVector4Animation() {}
	virtual ~UIVector4Animation() {}

	void SetParameter(const Vector4& start, const Vector4& end, float duration, UILoopMode loopMode = UILoopMode::Once) {
		m_start = start;
		m_end = end;
		m_duration = duration;
		m_loopMode = loopMode;
	}

	void SetFunc(const UIAnimationApplyFunc<Vector4>& func) { m_applyFunc = func; }

	void Play() override;
	void Update() override;

	Vector4 GetCurrentValue() const { return m_currentValue; }
};

//=======================================================
//   実際の機能クラス（UIBase.cpp 等でインスタンス化して使う）
//=======================================================

/* 色変更アニメーション */
class UIColorAnimation : public UIVector4Animation
{
public:
	UIColorAnimation();
};

/* 大きさ変更アニメーション */
class UIScaleAnimation : public UIVector3Animation
{
public:
	UIScaleAnimation();
};

/* 位置変更アニメーション*/
class UIPositionAnimation : public UIVector3Animation
{
public:
	UIPositionAnimation();
};