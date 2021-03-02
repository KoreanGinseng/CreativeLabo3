#pragma once
#include "SceneDefine.h"
#include "BackRender.h"

// ********************************************************************************
/// <summary>
/// ゲームシーンクラス
/// </summary>
// ********************************************************************************
class CGame : public MyApp::CScene
{
private:

	enum Phase
	{
		TurnAnimation,
		PlayerInput,
		PlayerCheck,
		EnemyInput,
		EnemyCheck,

	};

	CBackRender m_BackRender{ CBackRender::Brown };

public:

    explicit CGame(const IScene::InitData& init);

    ~CGame(void);

    void Update(void);

    void Render(void);
};

