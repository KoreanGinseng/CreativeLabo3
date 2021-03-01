#pragma once
#include "Button.h"


// ********************************************************************************
/// <summary>
/// 数字入力用ボタンクラス
/// </summary>
// ********************************************************************************
class CNumButtons
{
public:

    static constexpr int InvalidNum{ 100 };

private:

    bool    m_bSingle{ true };

    CButton m_BtnNums[10];

    Vector2 m_Pos;

    int     m_PushNum{ InvalidNum };

public:

    CNumButtons(bool bSingle);
    
    ~CNumButtons(void);

    void Update(void);

    void Render(void);

    int GetPushNum(void) const;

    int GetPushNumCount(void) const;

    void SetPos(const Vector2& pos);

    void SetPush(int n, bool bPush);
};

