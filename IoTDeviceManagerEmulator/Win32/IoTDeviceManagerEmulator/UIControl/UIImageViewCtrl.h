#pragma once


// CUIImageViewCtrl

class CUIImageViewCtrl : public CStatic
{
	DECLARE_DYNAMIC(CUIImageViewCtrl)

public:
	CUIImageViewCtrl();
	virtual ~CUIImageViewCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();

private:
    CImage  img_;
};


