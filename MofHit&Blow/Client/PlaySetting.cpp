#include "PlaySetting.h"

CPlaySetting::CPlaySetting(const IScene::InitData & init)
    : IScene(init)
{
    m_pFont    = CResourceManager::GetResourceFont("Main"  );
    m_pBtnFont = CResourceManager::GetResourceFont("Button");
    m_pFont->SetSize(70, 90);
    m_NumButton.SetPos(Vector2(100, 500));
    m_BtnCancel.SetButton(Vector2(650, 650), Vector2(300, 50), "Cancel");
    m_BtnPlay  .SetButton(Vector2(300, 650), Vector2(300, 50), "Play"  );
    m_BtnPlay  .SetPush(true);

    for (int i = 0; i < 3; i++)
    {
        Vector2 btnPos (300, 200);
        Vector2 btnSize( 50,  50);
        btnPos.x += i * (btnSize.x * 1.5f);
        m_BtnDigits[i].SetButton(btnPos, btnSize, std::to_string(3 + i));
    }
    m_BtnDigits[0].SetPush(true);

    for (int i = 0; i < 5; i++)
    {
        Vector2 btnPos (400, 300);
        Vector2 btnSize( 50,  50);
        btnPos.x += i * (btnSize.x * 1.5f);
        m_BtnNums[i].SetButton(btnPos, btnSize, "");
    }
    m_BtnNums[0].SetPush(true);
}

CPlaySetting::~CPlaySetting(void)
{
}

void CPlaySetting::Update(void)
{
    if (m_BtnCancel.IsMousePull())
    {
        ChangeScene(SceneName::Title);
    }

    m_BtnPlay.SetPush(m_NumButton.GetPushNumCount() != GetData().digit);

    if (!m_BtnPlay.IsPush() && m_BtnPlay.IsMousePull())
    {
        for (int i = 0; i < GetData().digit; i++)
        {
            GetData().answer[i] = atoi(m_BtnNums[i].GetText().c_str());
        }
        ChangeScene(SceneName::Game);
    }
    
    m_NumButton.Update();
    int pushNum = m_NumButton.GetPushNum();
    if (pushNum != CNumButtons::InvalidNum)
    {
        if (pushNum < 0)
        {
            int num = -pushNum;
            if (num == CNumButtons::InvalidNum)
            {
                num = 0;
            }
            for (int i = 0; i < GetData().digit; i++)
            {
                int check = atoi(m_BtnNums[i].GetText().c_str());
                if (num != check)
                {
                    continue;
                }
                m_BtnNums[m_ChoiceDigit].SetPush(false);
                m_ChoiceDigit = i;
                m_BtnNums[m_ChoiceDigit].SetPush(true);
                m_BtnNums[i].SetText("");
                break;
            }
        }
        else
        {
            std::string text = m_BtnNums[m_ChoiceDigit].GetText();
            if (text != "")
            {
                int check = atoi(text.c_str());
                m_NumButton.SetPush(check, false);
            }
            m_BtnNums[m_ChoiceDigit].SetText(std::to_string(pushNum));
            m_BtnNums[m_ChoiceDigit].SetPush(false);
            m_ChoiceDigit = MOF_MIN(m_ChoiceDigit + 1, GetData().digit - 1);
            m_BtnNums[m_ChoiceDigit].SetPush(true);
        }
    }

    for (int i = 0; i < GetData().digit; i++)
    {
        if (!m_BtnNums[i].IsMousePull())
        {
            continue;
        }
        m_BtnNums[i].SetPush(true);
        m_ChoiceDigit = i;
        for (int j = 0; j < GetData().digit; j++)
        {
            if (i == j) continue;
            m_BtnNums[j].SetPush(false);
        }
        break;
    }

    for (int i = 0; i < 3; i++)
    {
        if (!m_BtnDigits[i].IsMousePull())
        {
            continue;
        }
        m_BtnDigits[i].SetPush(true);
        GetData().digit = 3 + i;
        m_BtnNums[m_ChoiceDigit].SetPush(false);
        m_ChoiceDigit = 0;
        m_BtnNums[m_ChoiceDigit].SetPush(true);
        for (int j = 4; j >= GetData().digit; j--)
        {
            std::string text = m_BtnNums[j].GetText();
            if (text != "")
            {
                int check = atoi(text.c_str());
                m_NumButton.SetPush(check, false);
                m_BtnNums[j].SetText("");
            }
        }
        for (int j = 0; j < 3; j++)
        {
            if (i == j) continue;
            m_BtnDigits[j].SetPush(false);
        }
        break;
    }
}

void CPlaySetting::Render(void)
{
    m_BackRender.Render();

    m_pFont->RenderString(40, 40, MOF_COLOR_CBLACK, "Play Setting");
    m_pFont->RenderString(30, 30, MOF_COLOR_WHITE , "Play Setting");

    m_pBtnFont->RenderString(103, 203, MOF_COLOR_CBLACK, "ケタスウ : ");
    m_pBtnFont->RenderString(100, 200, MOF_COLOR_WHITE , "ケタスウ : ");

    m_pBtnFont->RenderString(103, 303, MOF_COLOR_CBLACK, "あなたのすうじ : ");
    m_pBtnFont->RenderString(100, 300, MOF_COLOR_WHITE , "あなたのすうじ : ");

    for (int i = 0; i < 3; i++)
    {
        m_BtnDigits[i].Render();
    }

    for (int i = 0; i < GetData().digit; i++)
    {
        m_BtnNums[i].Render();
    }

    m_NumButton.Render();

    m_BtnCancel.Render();
    m_BtnPlay  .Render();
}
