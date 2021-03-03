#pragma once
#include "SceneDefine.h"
#include "BackRender.h"
#include "ResourceManager.h"
#include "NumButtons.h"

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
		Result,
	}               m_Phase;
				    
	float           m_Timer;

	sip::CStopWatch m_StopWatch;

	CReFontPtr      m_pFont;

	CButton         m_BtnEnter;

	CNumButtons     m_NumButton    { true };

	CNumButtons     m_NumButtonMemo{ true };

	CBackRender     m_BackRender{ CBackRender::Brown };

public:

    explicit CGame(const IScene::InitData& init);

    ~CGame(void);

    void Update(void);

    void Render(void);
};

