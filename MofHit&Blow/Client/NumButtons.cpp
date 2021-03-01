#include "NumButtons.h"

CNumButtons::CNumButtons(bool bSingle)
{
    m_bSingle = bSingle;
    Vector2 btnSize(50, 50);
    for (int i = 0; i < 10; i++)
    {
        m_BtnNums[i].SetText(std::to_string(i));
        m_BtnNums[i].SetSize(btnSize);
    }
    SetPos(Vector2(100, 100));
}

CNumButtons::~CNumButtons(void)
{
}

void CNumButtons::Update(void)
{
    m_PushNum = InvalidNum;
    for (int i = 0; i < 10; i++)
    {
        if (!m_BtnNums[i].IsMousePull())
        {
            continue;
        }
        if (m_bSingle)
        {
            m_BtnNums[i].SetPush(!m_BtnNums[i].IsPush());
            m_PushNum = i * (m_BtnNums[i].IsPush() ? 1 : -1);
            if (m_PushNum == 0 && !m_BtnNums[i].IsPush())
            {
                m_PushNum = -InvalidNum;
            }
        }
        else
        {
            m_PushNum = i;
        }
        break;
    }
}

void CNumButtons::Render(void)
{
    for (int i = 0; i < 10; i++)
    {
        m_BtnNums[i].Render();
    }
}

void CNumButtons::SetPos(const Vector2 & pos)
{
    m_Pos = pos;
    float margin = m_BtnNums[0].GetRect().GetWidth() * 0.3f;
    for (int i = 0; i < 10; i++)
    {
        Vector2 btnPos = m_Pos;
        btnPos.x += (i % 5) * (m_BtnNums[i].GetRect().GetWidth()  + margin);
        btnPos.y += (i / 5) * (m_BtnNums[i].GetRect().GetHeight() + margin);
        m_BtnNums[i].SetPos(btnPos);
    }
}

void CNumButtons::SetPush(int n, bool bPush)
{
    m_BtnNums[n].SetPush(bPush);
}

int CNumButtons::GetPushNum(void) const
{
    return m_PushNum;
}

int CNumButtons::GetPushNumCount(void) const
{
    int cnt = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!m_BtnNums[i].IsPush())
        {
            continue;
        }
        cnt++;
    }
    return cnt;
}
