#include "Game.h"

CGame::CGame(const IScene::InitData & init)
    : IScene(init)
{
	m_pFont = CResourceManager::GetResourceFont("Button");
	m_BtnEnter.SetButton(Vector2(180, 450), Vector2(230, 50), "Check!");
	m_NumButton.SetPos(Vector2(100, 300));
	m_NumButtonMemo.SetPos(Vector2(100, 600));

	m_Timer = 300.0f;
	m_StopWatch.Start();
}

CGame::~CGame(void)
{
}

void CGame::Update(void)
{
	if (g_pInput->IsKeyPush(MOFKEY_F2))
	{
		ChangeScene(SceneName::Title);
	}

	m_StopWatch.Update();

	m_Timer -= m_StopWatch.GetTime();

	m_NumButton.Update();
	m_NumButtonMemo.Update();
}

void CGame::Render(void)
{
	m_BackRender.Render();
	
	m_pFont->RenderString(30, 30, MOF_COLOR_WHITE, "‚ ‚È‚½‚Ìƒ^[ƒ“");

	m_NumButton.Render();
	
	m_pFont->RenderString(50, 550, MOF_COLOR_WHITE, "ƒƒ‚");
	m_NumButtonMemo.Render();
	
	m_BtnEnter.Render();
}
