#include "BackRender.h"

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type"></param>
/// <created>いのうえ,2021/02/21</created>
/// <changed>いのうえ,2021/02/21</changed>
// ********************************************************************************
CBackRender::CBackRender(Type type)
{
    SetType(type);
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
/// <created>いのうえ,2021/02/21</created>
/// <changed>いのうえ,2021/02/21</changed>
// ********************************************************************************
CBackRender::~CBackRender(void)
{
}

// ********************************************************************************
/// <summary>
/// 描画
/// </summary>
/// <created>いのうえ,2021/02/21</created>
/// <changed>いのうえ,2021/02/21</changed>
// ********************************************************************************
void CBackRender::Render(void)
{
    static int scroll = 0;
    scroll++;

    int h   = m_pTexture->GetHeight();
    int w   = m_pTexture->GetWidth();
    int sch = g_pGraphics->GetTargetHeight();
    int scw = g_pGraphics->GetTargetWidth();
    for (float y = (scroll % h) - h; y < sch; y += h)
    {
        for (int x = 0; x < scw; x += w)
        {
            m_pTexture->Render(x, y);
        }
    }
}

// ********************************************************************************
/// <summary>
/// 種類の設定
/// </summary>
/// <param name="type">種類</param>
/// <created>いのうえ,2021/02/21</created>
/// <changed>いのうえ,2021/02/21</changed>
// ********************************************************************************
void CBackRender::SetType(Type type)
{
    std::string key[] = 
    {
        "Back_Blue",
        "Back_Brown",
        "Back_Gray",
        "Back_Green",
        "Back_Pink",
        "Back_Purple",
        "Back_Yellow",
    };
    m_pTexture = CResourceManager::GetTexture(key[type]);
}
