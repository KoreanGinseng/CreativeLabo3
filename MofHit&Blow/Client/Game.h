#pragma once
#include "SceneDefine.h"

// ********************************************************************************
/// <summary>
/// �Q�[���V�[���N���X
/// </summary>
// ********************************************************************************
class CGame : public MyApp::CScene
{
private:



public:

    explicit CGame(const IScene::InitData& init);

    ~CGame(void);

    void Update(void);

    void Render(void);
};

