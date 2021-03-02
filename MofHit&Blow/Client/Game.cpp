#include "Game.h"

CGame::CGame(const IScene::InitData & init)
    : IScene(init)
{
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


}

void CGame::Render(void)
{
	m_BackRender.Render();
}
