// Control/UIImageViewCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HP11X.h"

#include "UIImageViewCtrl.h"


// CUIImageViewCtrl

IMPLEMENT_DYNAMIC(CUIImageViewCtrl, CStatic)

CUIImageViewCtrl::CUIImageViewCtrl()
{
    img_.Load(_T("HUMAX_LOGO.bmp"));

}

CUIImageViewCtrl::~CUIImageViewCtrl()
{
}


BEGIN_MESSAGE_MAP(CUIImageViewCtrl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()



// CUIImageViewCtrl message handlers




void CUIImageViewCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages

    CRect rect;
    GetClientRect(rect);

    //img_.Draw(dc, CRect(0, 0, 320, 71));
    img_.Draw(dc, CRect(0, 0, img_.GetWidth(), img_.GetHeight()));

    //dc.FillSolidRect(rect, RGB(255, 255, 0));
}
