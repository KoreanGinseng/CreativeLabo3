#pragma once
#include "SceneDefine.h"
#include "ResourceManager.h"
#include "Button.h"
#include "NumButtons.h"
#include "BackRender.h"

// ********************************************************************************
/// <summary>
/// �v���C�Z�b�e�B���O�V�[��
/// </summary>
// ********************************************************************************
class CPlaySetting : public MyApp::CScene
{
private:

    CReFontPtr  m_pFont   { nullptr };
    CReFontPtr  m_pBtnFont{ nullptr };

    CButton     m_BtnCancel;
    CButton     m_BtnPlay;
    CButton     m_BtnDigits[3];
    CButton     m_BtnNums[5];

    int         m_ChoiceDigit{ 0 };

    CNumButtons m_NumButton{ true };

    CBackRender m_BackRender{ CBackRender::Purple };

public:

    explicit CPlaySetting(const IScene::InitData& init);

    ~CPlaySetting(void);

    void Update(void);

    void Render(void);

};

