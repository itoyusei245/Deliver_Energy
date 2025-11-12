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
   /* m_enemySpawnLevel.Init("Assets/modelData/level/enemySpawn.tkl",
        [&](LevelObjectData& objData)
        {
            if (objData.EqualObjectName(L"enemySpawn"))
            {
                StageManager::GetInstance()->CreateEnemyBox(objData.position, objData.rotation, objData.scale);
                return true;
            }
            return false;
        });*/

    m_stageSpawnLevel.Init("Assets/modelData/level/stageSpawn.tkl",
        [&](LevelObjectData& objData)
        {
            if (objData.EqualObjectName(L"enemySpawn"))
            {
                StageManager::GetInstance()->CreateEnemyBox(objData.position, objData.rotation, objData.scale);
                return true;
            }
            else if (objData.EqualObjectName(L"stageSpawn"))
            {
                StageManager::GetInstance()->CreateStageBox(objData.position, objData.rotation, objData.scale);
                return true;
            }
            return false;
        });
}