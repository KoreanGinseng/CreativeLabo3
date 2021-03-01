#pragma once
#include "ResourceManager.h"

// ********************************************************************************
/// <summary>
/// ボタンクラス
/// </summary>
// ********************************************************************************
class CButton
{
private:

    CReFontPtr  m_pFont{ nullptr };

    Vector2     m_Pos;
    Vector2     m_Size;

    std::string m_Text;

    CRectangle  m_FontRect;

    bool        m_bPush{ false };

public:

    CButton(void);
    
    ~CButton(void);

    void Render(void);

    bool IsMouseOver(void) const;

    bool IsMousePush(void) const;
    bool IsMouseHold(void) const;
    bool IsMousePull(void) const;

    bool IsPush(void) const;

    CRectangle GetRect(void) const;
    CRectangle GetFontRect(void) const;
    std::string GetText(void) const;

    void SetButton(const Vector2& pos, const Vector2& size, const std::string& text);

    void SetPos (const Vector2& pos);
    void SetSize(const Vector2& size);
    void SetText(const std::string& text);

    void SetPush(bool b);
};

