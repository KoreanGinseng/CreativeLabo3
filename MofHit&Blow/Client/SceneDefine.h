#pragma once
#include "SceneManager.h"

struct SceneData
{
    bool bMulti{ false };
    int  digit { 3 };
    int  answer[5];
};

enum class SceneName
{
    Title,
    PlaySetting,
    ServerSelect,
    MatchWait,
    Game,
};

using MyApp = sip::CSceneManager<SceneName, SceneData>;