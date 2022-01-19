//////////////////////////////////////////////////////////////////////////////
//  SliderBrightness.cpp - Brightness slider class
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThermGUI.h"
#include "SliderBrightness.h"
#include "ThermGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderBrightness

CSliderBrightness::CSliderBrightness()
{
    m_LeftButtonDown = FALSE;
}

CSliderBrightness::~CSliderBrightness()
{
}


BEGIN_MESSAGE_MAP(CSliderBrightness, CSliderCtrl)
	//{{AFX_MSG_MAP(CSliderBrightness)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderBrightness message handlers

void CSliderBrightness::OnLButtonDown(UINT nFlags, CPoint point) 
{
    m_LeftButtonDown = TRUE;
    
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CSliderBrightness::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_LeftButtonDown = FALSE;
    
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

void CSliderBrightness::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (m_LeftButtonDown) {
            // the user is scrolling
        CThermGUIDlg *pParent = (CThermGUIDlg*) GetParent();
        pParent->SetBrightness();
    }

	CSliderCtrl::OnMouseMove(nFlags, point);
}

void CSliderBrightness::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
        // the user is scrolling -- may get more than one key down
        //  before key up
    CThermGUIDlg *pParent = (CThermGUIDlg*) GetParent();
    pParent->SetBrightness();
	
	CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSliderBrightness::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
        // the user is scrolling -- may get more than one key down
        //  before key up
    CThermGUIDlg *pParent = (CThermGUIDlg*) GetParent();
    pParent->SetBrightness();
	
	CSliderCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}
