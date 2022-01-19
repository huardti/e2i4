//////////////////////////////////////////////////////////////////////////////
//  ThermGUI.h  -  ThermUsb sample for Cypress Semiconductor USB kit
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMGUI_H__C09D8BC8_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
#define AFX_THERMGUI_H__C09D8BC8_2E56_11D2_A15C_00C04FC209F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CThermGUIApp:
// See ThermGUI.cpp for the implementation of this class
//

class CThermGUIApp : public CWinApp
{
public:
	CThermGUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermGUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CThermGUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMGUI_H__C09D8BC8_2E56_11D2_A15C_00C04FC209F8__INCLUDED_)
