//////////////////////////////////////////////////////////////////////////////
// ThermGUIDlg.h - Cypress USB starter kit sample GUI app
// Demonstrates use of the WM_DEVICECHANGE message and the ThermUsb driver
//  
// Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMGUIDLG_H__C09D8BCA_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
#define AFX_THERMGUIDLG_H__C09D8BCA_2E56_11D2_A15C_00C04FC209F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SliderBrightness.h"

/////////////////////////////////////////////////////////////////////////////
// CThermGUIDlg dialog

class CThermGUIDlg : public CDialog
{
// Construction
public:
	CThermGUIDlg(CWnd* pParent = NULL);	// standard constructor

    VOID SetBrightness();
// Dialog Data
	//{{AFX_DATA(CThermGUIDlg)
	enum { IDD = IDD_THERMGUI_DIALOG };
	CSliderBrightness m_SliderBrightness;
	short	m_Brightness;
	CString	m_strBrightness;
	CString	m_strTemperature;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermGUIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON  m_hIcon;             // handle to the icon
    HANDLE m_hThermUsb;         // handle to ThermUsb device
    virtual VOID OnOK();        // handler for user hitting enter key

	// Generated message map functions
	//{{AFX_MSG(CThermGUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
        // handler for WM_DEVICE change
    afx_msg VOID OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMGUIDLG_H__C09D8BCA_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
