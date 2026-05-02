#include "stdafx.h"
#include "EventUI.h"
#include "Camera/EventCamera.h"

namespace {
    const char* const EVENT_TEX_PATH = "Assets/sprite/MoveUI.DDS";
    const float       EVENT_TEX_W = 1920.0f;
    const float       EVENT_TEX_H = 1080.0f;
    const Vector3     EVENT_POS = Vector3::Zero;
}

EventUI::EventUI()
{
    m_canvas = new UICanvas();
    m_icon = m_canvas->CreateUI<UIIcon>();
}

EventUI::~EventUI()
{
    delete m_canvas;
}

bool EventUI::Start()
{
    m_icon->Initialize(
        EVENT_TEX_PATH,
        EVENT_TEX_W, 
        EVENT_TEX_H,
        EVENT_POS,
        Vector3::One,
        Quaternion::Identity
    );

    return true;
}

void EventUI::Update()
{
    if (EventCamera::IsEventPlaying) {
        m_canvas->Update();
    }
}

void EventUI::Render(RenderContext& rc)
{
    if (EventCamera::IsEventPlaying) {
        m_canvas->Render(rc);
    }
}