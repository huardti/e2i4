//////////////////////////////////////////////////////////////////////////////
//  SliderBrightness.h - Brightness slider class
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLIDERBRIGHTNESS_H__C09D8BD6_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
#define AFX_SLIDERBRIGHTNESS_H__C09D8BD6_2E56_11D2_A15C_00C04FC209F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSliderBrightness window

class CSliderBrightness : public CSliderCtrl
{
// Construction
public:
	CSliderBrightness();

// Attributes
public:

// Operations
public:
    BOOLEAN m_LeftButtonDown;   // TRUE when left mouse button is down

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderBrightness)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSliderBrightness();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSliderBrightness)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERBRIGHTNESS_H__C09D8BD6_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
