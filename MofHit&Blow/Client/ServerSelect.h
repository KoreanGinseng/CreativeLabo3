#pragma once
#include "SceneDefine.h"

// ********************************************************************************
/// <summary>
/// �T�[�o�[�Z���N�g�V�[���N���X
/// </summary>
// ********************************************************************************
class CServerSelect : public MyApp::CScene
{
private:

public:

    explicit CServerSelect(const IScene::InitData& init);
    
    ~CServerSelect(void);

    void Update(void);

    void Render(void);
};

