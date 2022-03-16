//////////////////////////////////////////////////////////////////////////////
//  ThermGUIDlg.cpp - Cypress USB starter kit sample GUI app
//  Demonstrates use of the WM_DEVICECHANGE message and the ThermUsb driver
//  
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThermGUI.h"
#include "ThermGUIDlg.h"
#include "ThermUsbFunctions.h"
#include <dbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermGUIDlg dialog


    // CONSTRUCTOR
CThermGUIDlg::CThermGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThermGUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThermGUIDlg)
	m_Brightness = 0;
	m_strBrightness = _T("");
	m_strTemperature = _T("");
	//}}AFX_DATA_INIT
	    // note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        // init device handle
    m_hThermUsb = INVALID_HANDLE_VALUE;
}

void CThermGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThermGUIDlg)
	DDX_Control(pDX, IDSLIDE_BRIGHTNESS, m_SliderBrightness);
	DDX_Text(pDX, IDTEXT_BRIGHTNESS, m_strBrightness);
	DDX_Text(pDX, ID_TEMPERATURE, m_strTemperature);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThermGUIDlg, CDialog)
	//{{AFX_MSG_MAP(CThermGUIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDB_ABOUT, OnAbout)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermGUIDlg message handlers
/////////////////////////////////////////////////////////////////////////////
 
BOOL CThermGUIDlg::OnInitDialog()
{
    DEV_BROADCAST_HANDLE broadcastInterface;

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	    // Set the icon for this dialog.  The framework does this automatically
	    //  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

        // open a handle to the device    
    m_hThermUsb = CreateFile(
                "\\\\.\\ThermUsb0",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, 
                OPEN_EXISTING, 
                0, 
                NULL);
    
        // verify new handle
    if (m_hThermUsb == INVALID_HANDLE_VALUE) {
        CHAR strError[256];
        sprintf(strError, "Error: couldn't open %s, (Error %d)\n", 
            "\\\\.\\ThermUsb0", 
            GetLastError());
        AfxMessageBox(strError);
       m_SliderBrightness.EnableWindow(FALSE);
    }

        // set up broadcase interface for notification
    broadcastInterface.dbch_size = sizeof(DEV_BROADCAST_HANDLE);
    broadcastInterface.dbch_devicetype = DBT_DEVTYP_HANDLE;
    broadcastInterface.dbch_handle = m_hThermUsb;

        // register to receive close for this device handle
    HDEVNOTIFY diNotifyHandle = RegisterDeviceNotification(m_hWnd,
                                            &broadcastInterface,
                                            DEVICE_NOTIFY_WINDOW_HANDLE);

        // initialize timer -- 500 milliseconds
    SetTimer(ID_TEMPTIMER, 500, NULL);
        // set range for brightness slider
    m_SliderBrightness.SetRange(0, 15, FALSE);
        // set current position
    m_SliderBrightness.SetPos(15);
        // set brightness to max
    m_Brightness = 15;
        // set brightness string
    m_strBrightness.Format("Brightness Level %d/15", m_Brightness);
        // set temperature string
    m_strTemperature.Format("0 C\t00:00:00");

    UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThermGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThermGUIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// the system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CThermGUIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//////////////////////////////////////////////////////////////////////////////
// Additional Windows Message Handlers
//////////////////////////////////////////////////////////////////////////////
 
//////////////////////////////////////////////////////////////////////////////
// OnDeviceChange - called when device state changes
// Inputs:  nEventType - event describes what type of device change occured
//          dwData - event specific data
// Output:
// Notes:   WM_DEVICECHANGE allows an Windows app to more elegantly clean up
//////////////////////////////////////////////////////////////////////////////
VOID CThermGUIDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
    switch (nEventType) {
    case DBT_DEVICEARRIVAL:
        AfxMessageBox("DBT_DEVICEARRIVAL\nCreating new handle to device.");
            
            // close handle to ThermUsb if one exists (should never happen!)
        if (m_hThermUsb != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hThermUsb);
            m_SliderBrightness.m_LeftButtonDown = FALSE;
                // disable slider window
            m_SliderBrightness.EnableWindow(FALSE);
            GetDlgItem(ID_TEMPERATURE)->EnableWindow(FALSE);
            m_hThermUsb = INVALID_HANDLE_VALUE;
        }

            // open a handle to the device    
        m_hThermUsb = CreateFile(
                    "\\\\.\\ThermUsb0",
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, 
                    OPEN_EXISTING, 
                    0, 
                    NULL);
    
            // verify new handle
        if (m_hThermUsb == INVALID_HANDLE_VALUE) {
            CHAR strError[256];
            sprintf(strError, "Error: couldn't open %s, (Error %d)\n", 
                "\\\\.\\ThermUsb0", 
                GetLastError());
            AfxMessageBox(strError);
                // disable slider window
            m_SliderBrightness.EnableWindow(FALSE);
            GetDlgItem(ID_TEMPERATURE)->EnableWindow(FALSE);
        } else {
                // enable slider window
            m_SliderBrightness.EnableWindow(TRUE);
            GetDlgItem(ID_TEMPERATURE)->EnableWindow(TRUE);
                // initialize timer -- 500 milliseconds
            SetTimer(ID_TEMPTIMER, 500, NULL);
        }

        break;
    case DBT_DEVICEQUERYREMOVE:
        AfxMessageBox("DBT_DEVICEQUERYREMOVE not handled");
        break;
    case DBT_DEVICEQUERYREMOVEFAILED:
        AfxMessageBox("DBT_DEVICEQUERYREMOVEFAILED not handled");
        break;
    case DBT_DEVICEREMOVEPENDING:
        AfxMessageBox("DBT_DEVICEREMOVEPENDING not handled");
        break;
    case DBT_DEVICEREMOVECOMPLETE:
        AfxMessageBox("DBT_DEVICEREMOVECOMPLETE\nClosing handle to device.");
             // close handle to ThermUsb
        if (m_hThermUsb != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hThermUsb);
            m_SliderBrightness.m_LeftButtonDown = FALSE;
                // disable slider window
            m_SliderBrightness.EnableWindow(FALSE);
            GetDlgItem(ID_TEMPERATURE)->EnableWindow(FALSE);
            KillTimer(ID_TEMPTIMER);
            m_hThermUsb = INVALID_HANDLE_VALUE;
        }
        break;
    case DBT_DEVICETYPESPECIFIC:
        AfxMessageBox("DBT_DEVICETYPESPECIFIC not handled");
        break;
    case DBT_CONFIGCHANGED:
        AfxMessageBox("DBT_CONFIGCHANGED not handled");
        break;
   }
}

//////////////////////////////////////////////////////////////////////////////
// SetBrightness - set green LED brightness based on scrollbar
// Inputs:  
// Output:
// Notes:  called by m_SliderBrightness
//////////////////////////////////////////////////////////////////////////////
VOID CThermGUIDlg::SetBrightness()
{
    m_Brightness = m_SliderBrightness.GetPos();
    m_strBrightness.Format("Brightness Level %d/15", m_Brightness);
    UpdateData(FALSE);
        // set LED brightness
    if (!SetLedBrightness(m_hThermUsb, m_Brightness)) {
            // if we fail then we couldn't unclick the mouse in time
        m_SliderBrightness.m_LeftButtonDown = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////////
// OnOK - called when user hits enter key
// Inputs:  
// Output:
// Notes:   Keep the dialog from closing on "enter"
//////////////////////////////////////////////////////////////////////////////
void CThermGUIDlg::OnOK()
{
     return;
}

//////////////////////////////////////////////////////////////////////////////
// OnDestroy - overloaded destroy member for WM_DESTROY
// Inputs:  
// Output:
// Notes:   
//////////////////////////////////////////////////////////////////////////////
void CThermGUIDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    
        // close handle to ThermUsb
    if (m_hThermUsb != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hThermUsb);
        m_hThermUsb = INVALID_HANDLE_VALUE;
    }

}

//////////////////////////////////////////////////////////////////////////////
// OnAbout - display about dialog
// Inputs:  
// Output:
// Notes:   
//////////////////////////////////////////////////////////////////////////////
void CThermGUIDlg::OnAbout() 
{
    CAboutDlg dlg;  // default about dialog
    dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////////
// OnTimer - timer callback responds to WM_TIMER 
// Inputs: nIDEvent - identifies the timer triggering the message
// Output:
// Notes: setup in OnInitDialog
//////////////////////////////////////////////////////////////////////////////
void CThermGUIDlg::OnTimer(UINT nIDEvent) 
{
    LONG buttonState;   // hardware push button on the Cypress board
    USHORT rawTemp;     // raw temp -- some USHORT
    float trueTemp;     // temperature in degrees F or C
    CTime time =        // can't have temperature without the time!
        CTime::GetCurrentTime();


        // call down to driver for button state
    if (-1 == (buttonState = GetButtonState(m_hThermUsb))) {
        KillTimer(ID_TEMPTIMER);
        GetDlgItem(ID_TEMPERATURE)->EnableWindow(FALSE);
        return;
    }

        // call down to the driver for temperature
    if (!GetRawTemperature(m_hThermUsb, &rawTemp)) {
        KillTimer(ID_TEMPTIMER);
        GetDlgItem(ID_TEMPERATURE)->EnableWindow(FALSE);
        return;
    }

        // display temperature
    if (buttonState) {
            // convert to degrees Celcius
        trueTemp = GetTempCFromRawTemp(rawTemp);
        m_strTemperature.Format("%.1f C\t\t%02d:%02d:%02d", 
            trueTemp,
            time.GetHour(),
            time.GetMinute(),
            time.GetSecond());
    } else {
            // convert to degrees Fahrenheit
        trueTemp = GetTempFFromRawTemp(rawTemp);
        m_strTemperature.Format("%.1f F\t\t%02d:%02d:%02d", 
            trueTemp,
            time.GetHour(),
            time.GetMinute(),
            time.GetSecond());
    }

    UpdateData(FALSE);

        // call base class handler
	CDialog::OnTimer(nIDEvent);
}
