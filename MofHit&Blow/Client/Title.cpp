#include "Title.h"

CTitle::CTitle(const IScene::InitData & init)
    : IScene(init)
{
    m_pFont = CResourceManager::GetResourceFont("Main");
    m_pFont->SetSize(70, 90);
    m_pFont->CalculateStringRect(0, 0, "Hit & Blow", m_TitleRect);
    m_TitleRect.SetPosition(
        Vector2((g_pGraphics->GetTargetWidth() - m_TitleRect.GetWidth()) * 0.5f, 100)
    );

    Vector2 btnSize(300, 50);
    float   btnPosX = (g_pGraphics->GetTargetWidth() - btnSize.x) * 0.5f;
    float   btnPosY = g_pGraphics->GetTargetHeight() * 0.5f;
    m_BtnSingle.SetButton(Vector2(btnPosX, btnPosY + btnSize.y * 0.0f), btnSize, "Single");
    m_BtnMulti .SetButton(Vector2(btnPosX, btnPosY + btnSize.y * 1.5f), btnSize, "Multi");
    m_BtnExit  .SetButton(Vector2(btnPosX, btnPosY + btnSize.y * 3.0f), btnSize, "Exit");
}

CTitle::~CTitle(void)
{
}

void CTitle::Update(void)
{
    if (m_BtnExit.IsMousePull())
    {
        PostQuitMessage(0);
    }

    if (m_BtnSingle.IsMousePull())
    {
        ChangeScene(SceneName::PlaySetting);
    }

    if (m_BtnMulti.IsMousePull())
    {
        ChangeScene(SceneName::ServerSelect);
    }
}

void CTitle::Render(void)
{
    m_BackRender.Render();

    m_pFont->RenderString(m_TitleRect.Left + 10, m_TitleRect.Top + 10, MOF_COLOR_CBLACK, "Hit & Blow");
    m_pFont->RenderString(m_TitleRect.Left     , m_TitleRect.Top     , MOF_COLOR_WHITE , "Hit & Blow");

    m_BtnSingle.Render();
    m_BtnMulti.Render();
    m_BtnExit.Render();
}
