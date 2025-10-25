#include "stdafx.h"
#include "TriggerBox.h"
#include "StageManager.h"
TriggerBox::TriggerBox()
{
}

TriggerBox::~TriggerBox()
{
}
void TriggerBox::Init()
{
    m_levelRender.Init("Assets/modelData/level/enemySpawn.tkl",
        [&](LevelObjectData& objData)
        {
            if (objData.EqualObjectName(L"enemySpawn") == true)
            {
                StageManager::GetInstance()->CreateBox(objData.position, objData.rotation, objData.scale);
                return true;
            }
        });
}