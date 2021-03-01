#pragma once
#include "SceneDefine.h"
#include "ResourceManager.h"
#include "Button.h"
#include "BackRender.h"

// ********************************************************************************
/// <summary>
/// タイトルシーンクラス
/// </summary>
// ********************************************************************************
class CTitle : public MyApp::CScene
{
private:

    CReFontPtr  m_pFont{ nullptr };
    CRectangle  m_TitleRect;

    CButton     m_BtnSingle;
    CButton     m_BtnMulti;
    CButton     m_BtnExit;

    CBackRender m_BackRender{ CBackRender::Green };

public:

    explicit CTitle(const IScene::InitData& init);
    
    ~CTitle(void);

    void Update(void);

    void Render(void);
};

