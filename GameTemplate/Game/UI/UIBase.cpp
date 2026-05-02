/**
 * @file UIBase.cpp
 * @brief UIの基本的な処理をするクラス群
 */
#include "stdafx.h"
#include "UIBase.h"
#include "UIAnimation.h"

 // ============================================
 // UIBase 基底クラス
 // ============================================
void UIBase::SetUIAnimation(std::unique_ptr<UIAnimationBase> animation)
{
	animation->SetUI(this);
	uiAnimationList_.push_back(std::move(animation));
}

// ============================================
// 画像を使うUI関連 (UIImage)
// ============================================
UIImage::UIImage() {}
UIImage::~UIImage() {}

bool UIImage::Start() { return true; }
void UIImage::Update() {}
void UIImage::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}

// ============================================
// ゲージUI (UIGauge)
// ============================================
UIGauge::UIGauge() {}
UIGauge::~UIGauge() {}

bool UIGauge::Start() { return true; }

void UIGauge::Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation)
{
	m_transform.m_localPosition = position;
	m_transform.m_localScale = scale;
	m_transform.m_localRotation = rotation;

	m_isCircle = false;

	m_spriteRender.Init(assetName, width, height);
	m_spriteRender.SetPosition(position);
	m_spriteRender.SetScale(scale);
	m_spriteRender.SetRotation(rotation);
	m_spriteRender.Update();
}

void UIGauge::CircleInitialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation)
{
	m_transform.m_localPosition = position;
	m_transform.m_localScale = scale;
	m_transform.m_localRotation = rotation;

	m_isCircle = true;

	m_circleProgress = 1.0f;
	m_circleSize = 1.0f;
	m_spriteRender.CircleInit(m_circleProgress, m_circleSize, assetName, width, height, AlphaBlendMode_Trans);

	m_spriteRender.SetPosition(position);
	m_spriteRender.SetScale(scale);
	m_spriteRender.SetRotation(rotation);
	m_spriteRender.Update();
}

void UIGauge::SetProgress(float progress)
{
	// 0.0 ～ 1.0 の間に制限する
	progress = max(0.0f, min(progress, 1.0f));

	if (m_isCircle) {
		m_circleProgress = progress;
		m_spriteRender.SetCircleProgress(progress);
	}
	else {
		// 通常のゲージの場合はXスケールを縮めることでゲージ表現とする
		m_transform.m_localScale.x = progress;
	}
}

void UIGauge::Update()
{
	for (auto& ui : uiAnimationList_) {
		UpdateAnimation();
		ui->Update();
	}

	m_transform.UpdateTransform();
	m_spriteRender.SetPosition(m_transform.m_position);
	m_spriteRender.SetScale(m_transform.m_scale);
	m_spriteRender.SetRotation(m_transform.m_rotation);
	m_spriteRender.Update();
}

void UIGauge::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}

// ============================================
// アイコンUI (UIIcon)
// ============================================
UIIcon::UIIcon() {}
UIIcon::~UIIcon() {}

bool UIIcon::Start() { return true; }

void UIIcon::Initialize(const char* assetName, const float width, const float height, const Vector3& position, const Vector3& scale, const Quaternion& rotation)
{
	m_transform.m_localPosition = position;
	m_transform.m_localScale = scale;
	m_transform.m_localRotation = rotation;

	m_spriteRender.Init(assetName, width, height);
	m_spriteRender.SetPosition(position);
	m_spriteRender.SetScale(scale);
	m_spriteRender.SetRotation(rotation);
	m_spriteRender.Update();
}

void UIIcon::Update()
{
	for (auto& ui : uiAnimationList_) {
		UpdateAnimation();
		ui->Update();
	}

	m_transform.UpdateTransform();
	m_spriteRender.SetPosition(m_transform.m_position);
	m_spriteRender.SetScale(m_transform.m_scale);
	m_spriteRender.SetRotation(m_transform.m_rotation);
	m_spriteRender.SetMulColor(color_);
	m_spriteRender.Update();
}

void UIIcon::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}

// ============================================
// 文字を使うUI関連 (UIText)
// ============================================
UIText::UIText() {}
UIText::~UIText() {}

bool UIText::Start() { return true; }
void UIText::Update() {}
void UIText::Render(RenderContext& rc) {}

// ============================================
// 数字描画UI (UINumber)
// ============================================
UINumber::UINumber() {}
UINumber::~UINumber() {}

bool UINumber::Start() { return true; }

void UINumber::Initialize(const char* dirPath, const char* ext, const float digitWidth, const float digitHeight, const Vector3& position, float digitSpacing)
{
	m_dirPath = dirPath;
	m_ext = ext;
	m_digitWidth = digitWidth;
	m_digitHeight = digitHeight;
	m_digitSpacing = digitSpacing;
	m_transform.m_localPosition = position;
}

void UINumber::SetNumber(int number)
{
	// 数値が変わっていない場合は無駄な更新処理を省く
	if (m_currentNumber == number) return;
	m_currentNumber = number;

	// 数字を文字列にして桁ごとに分解
	std::string numStr = std::to_string(number);

	// ゼロ埋め処理（例： padLength=3 で number=5 なら "005" にする）
	if (m_isZeroPadding && numStr.length() < m_padLength) {
		numStr.insert(0, m_padLength - numStr.length(), '0');
	}

	// 必要な桁数分だけ SpriteRender を確保する
	while (m_digitSprites.size() < numStr.length()) {
		m_digitSprites.push_back(std::make_unique<SpriteRender>());
	}

	// 各桁の画像ロードと座標設定
	for (size_t i = 0; i < numStr.length(); ++i) {
		std::string filePath = m_dirPath + numStr[i] + m_ext; // 例: "Assets/sprite/" + "5" + ".DDS"

		m_digitSprites[i]->Init(filePath.c_str(), m_digitWidth, m_digitHeight);

		// 親の座標を基準に、右にズラして配置
		Vector3 digitPos = m_transform.m_position; // グローバル座標を使用
		digitPos.x += i * m_digitSpacing;

		m_digitSprites[i]->SetPosition(digitPos);
		m_digitSprites[i]->Update();
	}

	// もし以前より桁数が減った場合、配列のサイズを合わせて余分な桁を消す
	if (m_digitSprites.size() > numStr.length()) {
		m_digitSprites.resize(numStr.length());
	}
}

void UINumber::Update()
{
	for (auto& ui : uiAnimationList_) {
		UpdateAnimation();
		ui->Update();
	}

	m_transform.UpdateTransform();

	// UINumber自体の座標が動いた場合、子（各桁）の座標も更新する
	for (size_t i = 0; i < m_digitSprites.size(); ++i) {
		Vector3 digitPos = m_transform.m_position;
		digitPos.x += i * m_digitSpacing;
		m_digitSprites[i]->SetPosition(digitPos);
		m_digitSprites[i]->Update();
	}
}

void UINumber::Render(RenderContext& rc)
{
	for (auto& sprite : m_digitSprites) {
		sprite->Draw(rc);
	}
}

// ============================================
// キャンバス (UICanvas)
// ============================================
UICanvas::UICanvas()
{
	m_uiList.clear();
}

UICanvas::~UICanvas()
{
	for (auto* ui : m_uiList) {
		// トランスフォームの親子関係を解除
		m_transform.RemoveChild(&ui->m_transform);
		// キャンバス上にあるUIを削除
		delete ui;
		ui = nullptr;
	}
	m_uiList.clear();
}

bool UICanvas::Start()
{
	return true;
}

void UICanvas::Update()
{
	for (auto& ui : uiAnimationList_) {
		UpdateAnimation();
		ui->Update();
	}

	m_transform.UpdateTransform();

	for (auto* ui : m_uiList) {
		if (ui->isUpdate) {
			ui->Update();
		}
	}
}

void UICanvas::Render(RenderContext& rc)
{
	for (auto* ui : m_uiList) {
		if (ui->isDraw) {
			ui->Render(rc);
		}
	}
}