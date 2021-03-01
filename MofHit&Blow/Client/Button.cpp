#include "Button.h"

CButton::CButton(void)
{
    m_pFont = CResourceManager::GetResourceFont("Button");
    m_pFont->SetSize(30, 40);
}

CButton::~CButton(void)
{
}

void CButton::Render(void)
{
    CRectangle rect     = GetRect();
    MofU32 rectColor    = MOF_COLOR_HWHITE;
    MofU32 boarderColor = MOF_COLOR_BLACK;
    MofU32 fontColor    = MOF_COLOR_CWHITE;
    if (IsMouseOver())
    {
        boarderColor = MOF_COLOR_BLUE;
    }
    if (IsMouseHold())
    {
        rectColor = MOF_COLOR_CBLACK;
    }
    if (m_bPush)
    {
        rectColor = MOF_COLOR_CBLACK;
    }

    float exp = MOF_MIN(m_Size.y, m_Size.x);
    rect.Expansion( exp * 0.1f);
    CGraphicsUtilities::RenderFillRect(rect, boarderColor);
    rect.Expansion(-exp * 0.1f);
    CGraphicsUtilities::RenderFillRect(rect, rectColor);

    Vector2 pos(m_Pos);
    pos.x += (rect.GetWidth()  - m_FontRect.GetWidth() ) * 0.5f;
    pos.y += (rect.GetHeight() - m_FontRect.GetHeight()) * 0.5f;
    m_pFont->RenderString(pos.x, pos.y, fontColor, m_Text.c_str());
}

bool CButton::IsMouseOver(void) const
{
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    return GetRect().CollisionPoint(mp);
}

bool CButton::IsMousePush(void) const
{
    return g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON) && IsMouseOver();
}

bool CButton::IsMouseHold(void) const
{
    return g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON) && IsMouseOver();
}

bool CButton::IsMousePull(void) const
{
    return g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON) && IsMouseOver();
}

bool CButton::IsPush(void) const
{
    return m_bPush;
}

CRectangle CButton::GetRect(void) const
{
    return CRectangle(m_Pos, m_Pos + m_Size);
}

CRectangle CButton::GetFontRect(void) const
{
    return m_FontRect;
}

std::string CButton::GetText(void) const
{
    return m_Text;
}

void CButton::SetButton(const Vector2 & pos, const Vector2 & size, const std::string & text)
{
    SetPos(pos);
    SetSize(size);
    SetText(text);
}

void CButton::SetPos(const Vector2 & pos)
{
    m_Pos = pos;
}

void CButton::SetSize(const Vector2 & size)
{
    m_Size = size;
}

void CButton::SetText(const std::string & text)
{
    m_Text = text;
    m_pFont->CalculateStringRect(0, 0, m_Text.c_str(), m_FontRect);
}

void CButton::SetPush(bool b)
{
    m_bPush = b;
}
