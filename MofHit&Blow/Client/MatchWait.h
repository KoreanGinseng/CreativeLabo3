#pragma once
#include "SceneDefine.h"

// ********************************************************************************
/// <summary>
/// マッチウェイトシーン
/// </summary>
// ********************************************************************************
class CMatchWait : public MyApp::CScene
{
private:



public:

    explicit CMatchWait(const IScene::InitData& init);

    ~CMatchWait(void);

    void Update(void);

    void Render(void);

};

