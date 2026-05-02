/**
 * SpriteAnimation.cpp
 * SpriteRenderをアニメーションさせる機能群
 */
#include "stdafx.h"
#include "UIAnimation.h"
#include "UIBase.h"

 // =========================================================
 // Vector2 Animation の実装
 // =========================================================
void UIVector2Animation::Play() {
	m_currentTime = 0.0f;
	m_isPlaying = true;
	m_isReverse = false;
	m_currentValue = m_start;
}

void UIVector2Animation::Update() {
	if (!m_isPlaying || m_duration <= 0.0f) return;

	float dt = g_gameTime->GetFrameDeltaTime();
	m_currentTime += m_isReverse ? -dt : dt;

	float t = m_currentTime / m_duration;
	bool finished = false;

	if (t >= 1.0f && !m_isReverse) { t = 1.0f; finished = true; }
	else if (t <= 0.0f && m_isReverse) { t = 0.0f; finished = true; }

	// 線形補間(Lerp)
	m_currentValue.x = m_start.x + (m_end.x - m_start.x) * t;
	m_currentValue.y = m_start.y + (m_end.y - m_start.y) * t;

	if (m_applyFunc) m_applyFunc(m_currentValue);

	if (finished) {
		if (m_loopMode == UILoopMode::Once) { m_isPlaying = false; }
		else if (m_loopMode == UILoopMode::Loop) { m_currentTime = 0.0f; }
		else if (m_loopMode == UILoopMode::PingPong) { m_isReverse = !m_isReverse; }
	}
}

// =========================================================
// Vector3 Animation の実装
// =========================================================
void UIVector3Animation::Play() {
	m_currentTime = 0.0f;
	m_isPlaying = true;
	m_isReverse = false;
	m_currentValue = m_start;
}

void UIVector3Animation::Update() {
	if (!m_isPlaying || m_duration <= 0.0f) return;

	float dt = g_gameTime->GetFrameDeltaTime();
	m_currentTime += m_isReverse ? -dt : dt;

	float t = m_currentTime / m_duration;
	bool finished = false;

	if (t >= 1.0f && !m_isReverse) { t = 1.0f; finished = true; }
	else if (t <= 0.0f && m_isReverse) { t = 0.0f; finished = true; }

	// 線形補間
	m_currentValue.x = m_start.x + (m_end.x - m_start.x) * t;
	m_currentValue.y = m_start.y + (m_end.y - m_start.y) * t;
	m_currentValue.z = m_start.z + (m_end.z - m_start.z) * t;

	if (m_applyFunc) m_applyFunc(m_currentValue);

	if (finished) {
		if (m_loopMode == UILoopMode::Once) { m_isPlaying = false; }
		else if (m_loopMode == UILoopMode::Loop) { m_currentTime = 0.0f; }
		else if (m_loopMode == UILoopMode::PingPong) { m_isReverse = !m_isReverse; }
	}
}

// =========================================================
// Vector4 Animation の実装
// =========================================================
void UIVector4Animation::Play() {
	m_currentTime = 0.0f;
	m_isPlaying = true;
	m_isReverse = false;
	m_currentValue = m_start;
}

void UIVector4Animation::Update() {
	if (!m_isPlaying || m_duration <= 0.0f) return;

	float dt = g_gameTime->GetFrameDeltaTime();
	m_currentTime += m_isReverse ? -dt : dt;

	float t = m_currentTime / m_duration;
	bool finished = false;

	if (t >= 1.0f && !m_isReverse) { t = 1.0f; finished = true; }
	else if (t <= 0.0f && m_isReverse) { t = 0.0f; finished = true; }

	// 線形補間
	m_currentValue.x = m_start.x + (m_end.x - m_start.x) * t;
	m_currentValue.y = m_start.y + (m_end.y - m_start.y) * t;
	m_currentValue.z = m_start.z + (m_end.z - m_start.z) * t;
	m_currentValue.w = m_start.w + (m_end.w - m_start.w) * t;

	if (m_applyFunc) m_applyFunc(m_currentValue);

	if (finished) {
		if (m_loopMode == UILoopMode::Once) { m_isPlaying = false; }
		else if (m_loopMode == UILoopMode::Loop) { m_currentTime = 0.0f; }
		else if (m_loopMode == UILoopMode::PingPong) { m_isReverse = !m_isReverse; }
	}
}

// =========================================================
// 個別アニメーションの設定
// =========================================================

UIColorAnimation::UIColorAnimation()
{
	SetFunc([&](Vector4 v) {
		m_ui->color_ = v;
		});
}

UIScaleAnimation::UIScaleAnimation()
{
	SetFunc([&](Vector3 s) {
		m_ui->m_transform.m_localScale = s;
		m_ui->m_transform.UpdateTransform();
		});
}

UIPositionAnimation::UIPositionAnimation()
{
	SetFunc([&](Vector3 p) {
		m_ui->m_transform.m_localPosition = p;
		m_ui->m_transform.UpdateTransform();
		});
}