// DialHelperDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialHelper.h"
#include "DialHelperDlg.h"
#include "../CommenCode/CXKUsername.h" 
#include "../CommenCode/Shortcut.h"
#include <Ras.h>
#include <RasDlg.h>
#include <RasError.h>
#pragma comment(lib, "Rasapi32.lib") 
#include "UserControl.h"

#define __TIME_FORMAT _T("%H:%M:%S")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static HRASCONN hRasConn = NULL;

VOID WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	//TRACE("RasDialFunc\n");
	if (unMsg != WM_RASDIALEVENT)
		TRACE("Strange event!\n");
	theApp.PostDialMessage(WM_DIAL_FUNC, rasconnstate, dwError);
	//TRACE("RasDialFunc - exit\n");
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum
	{
		IDD = IDD_ABOUTBOX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDialHelperDlg �Ի���




CDialHelperDlg::CDialHelperDlg(CWnd* pParent /*=NULL*/) : CDialog(CDialHelperDlg::IDD, pParent),
m_sEntryName(_T("")), m_iUserCount(1), m_iCurrentUser(1),
m_sUserName(_T("")), m_sPassword(_T("")), m_bDialOnStart(FALSE),
m_bAutoReDial(FALSE), m_iTimeout(30), m_bMinimizedOnConnect(FALSE),
m_bHangUpOnExit(FALSE)
/*, m_bEnableLog(FALSE)*/,
m_StatusText(_T("")), sText(_T("")),
sLastError(_T(""))
, bIsTimingDial(FALSE)
, codtDialTime(COleDateTime::GetCurrentTime())
, strDialTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTODIAL, m_bDialOnStart);
	DDX_Check(pDX, IDC_CHECK_REDIAL, m_bAutoReDial);
	DDX_Text(pDX, IDC_EDIT_REDIAL_TIME, m_iTimeout);
	DDX_Check(pDX, IDC_CHECK_ONLINE_MINSIZE, m_bMinimizedOnConnect);
	DDX_Check(pDX, IDC_CHECK_HANGUP, m_bHangUpOnExit);
	/*DDX_Check(pDX, IDC_CHECK_LOG, m_bEnableLog);*/
	DDV_MaxChars(pDX, m_sUserName, 30);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_sPassword);
	DDV_MaxChars(pDX, m_sPassword, 20);
	DDX_Text(pDX, IDC_EDIT_ENTRY_NAME, m_sEntryName);
	DDV_MaxChars(pDX, m_sEntryName, 16);
	DDX_Text(pDX, IDC_STATUS, m_StatusText);
	/*DDX_Control(pDX, DC_PROGRESS_TIMEOUT, m_ProgressTimeout);*/
	DDX_Control(pDX, IDC_CBO_USERNAME, CB_USERNAME);
	DDX_Text(pDX, IDC_CBO_USERNAME, m_sUserName);
	DDX_Text(pDX, IDC_TEXT, sText);
	DDX_Text(pDX, IDC_LastError, sLastError);
	DDX_Check(pDX, chkTimeingDial, bIsTimingDial);
	DDX_DateTimeCtrl(pDX, dtDialTime, codtDialTime);
}

BEGIN_MESSAGE_MAP(CDialHelperDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_CHECK_AUTODIAL, &CDialHelperDlg::OnBnClickedCheckAutodial)
	ON_BN_CLICKED(IDC_CHECK_REDIAL, &CDialHelperDlg::OnBnClickedCheckRedial)
	ON_BN_CLICKED(IDC_CHECK_ONLINE_MINSIZE, &CDialHelperDlg::OnBnClickedCheckOnlineMinsize)
	ON_BN_CLICKED(IDC_CHECK_HANGUP, &CDialHelperDlg::OnBnClickedCheckHangup)
	ON_BN_CLICKED(IDC_CHECK_LOG, &CDialHelperDlg::OnBnClickedCheckLog)
	ON_EN_CHANGE(IDC_EDIT_REDIAL_TIME, &CDialHelperDlg::OnEnChangeEditRedialTime)
	ON_BN_CLICKED(IDC_BUTTON_OPENLOG, &CDialHelperDlg::OnBnClickedButtonOpenlog)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CDialHelperDlg::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CDialHelperDlg::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_PROV_EDIT, &CDialHelperDlg::OnBnClickedButtonProvEdit)
	ON_BN_CLICKED(IDC_DIAL, &CDialHelperDlg::OnDial)
	ON_MESSAGE(WM_DIAL_FUNC, OnDialFunc)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CBO_USERNAME, &CDialHelperDlg::OnCbnSelchangeCboUsername)
	ON_BN_CLICKED(IDC_BUTTON_USERCONTROL, &CDialHelperDlg::OnBnClickedButtonUsercontrol)
	ON_BN_CLICKED(IDC_HANGUP, &CDialHelperDlg::OnBnClickedHangup)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDialHelperDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(btnAutoShortCut, &CDialHelperDlg::OnBnClickedbtnautoshortcut)
	ON_BN_CLICKED(chkTimeingDial, &CDialHelperDlg::OnBnClickedchktimeingdial)
	ON_NOTIFY(DTN_DATETIMECHANGE, dtDialTime, &CDialHelperDlg::OnDtnDatetimechangedtdialtime)
	ON_BN_CLICKED(btnSaveIni, &CDialHelperDlg::OnBnClickedbtnsaveini)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialHelperDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDialHelperDlg ��Ϣ�������

BOOL CDialHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	// 	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	// 	ASSERT(IDM_ABOUTBOX < 0xF000);
	// 
	// 	CMenu* pSysMenu = GetSystemMenu(FALSE);
	// 	if (pSysMenu != NULL)
	// 	{
	// 		CString strAboutMenu;
	// 		strAboutMenu.LoadString(IDS_ABOUTBOX);
	// 		if (!strAboutMenu.IsEmpty())
	// 		{
	// 			pSysMenu->AppendMenu(MF_SEPARATOR);
	// 			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	// 		}
	// 	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	// TODO: �ڴ���Ӷ���ĳ�ʼ������ 
	this->SetWindowText(CAPTION_NAME);
	
	
	//cti.Create(this,WM_ICON_NOTIFY,CAPTION_NAME,
	//			(HICON)LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON,16, 16, 0),NULL);
	//��ʼ��COM
	CoInitialize(NULL);

	LoadAllFromIni();

	LoadAllUserInfo();

	LoadCurrentUserInfo(m_iCurrentUser);

	OnBnClickedButtonOption();



	if (!QueryConnections(m_sEntryName))
	{
		//TRACE("not find the entry");
		CreatEntry(m_sEntryName);
	}

	UpdateData(FALSE);

	SetDlgItemText(btnAutoShortCut,
		CShortcut::Exist(CSIDL_COMMON_STARTUP, TEXT("�����������.lnk")) ?
		TEXT("ɾ��������") :
	TEXT("����������"));

	bDialNow = FALSE;
	bInOnTimer = FALSE;
	bConnected = FALSE;

	//��������ʱ�Զ�����
	bDialing = m_bDialOnStart || theApp.bAUTO;

	uTimer = SetTimer(1, 1000, NULL);

	

	if (bIsTimingDial)
	{
		uTimer2=SetTimer(2,1000,NULL);
	}
	else
	{
		KillTimer(2);
	}


	SetButtonsState();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDialHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDialHelperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND,
			reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDialHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��������
BOOL CDialHelperDlg::LoadAllFromIni()
{
	UpdateData();
	m_sEntryName	= theApp.GetIniString(gchMain, gchEntryName, _T("�������"));
	m_iUserCount	= _ttol(theApp.GetIniString(gchMain, gchUserCount, _T("1")));
	m_iCurrentUser	= _ttol(theApp.GetIniString(gchMain, gchCurrentUser,_T("1")));
	m_lLastTimeC	= _ttol(theApp.GetIniString(gchMain, gchLastTimeC, _T("0")));
	m_bDialOnStart	= _ttol(theApp.GetIniString(gchMain, gchDialOnStart,_T("0")));
	m_bAutoReDial	= _ttol(theApp.GetIniString(gchMain, gchAutoReDial, _T("0")));
	m_iTimeout		= _ttol(theApp.GetIniString(gchMain, gchTimeout, _T("30")));
	m_bMinimizedOnConnect = _ttol(theApp.GetIniString(gchMain,gchMinimizeOnConnect, _T("0")));
	m_bHangUpOnExit = _ttol(theApp.GetIniString(gchMain, gchHangUpOnExit,_T("0")));
	bIsTimingDial	=_ttol(theApp.GetIniString(gchMain, gchTimingDial,_T("0")));
	strDialTime		=theApp.GetIniString(gchMain,gchDialTime,_T("00:00:00"));
	codtDialTime.ParseDateTime(strDialTime,VAR_TIMEVALUEONLY);
	UpdateData(FALSE);
	return TRUE;
}
//����LastTimeC
BOOL CDialHelperDlg::SaveLastTimeC()
{
	theApp.SetIniString(gchMain, gchLastTimeC, L2S(m_lLastTimeC));
	return TRUE;
}
//��������
BOOL CDialHelperDlg::SaveAllToIni()
{
	UpdateData();

	theApp.SetIniString(gchMain, gchEntryName,		m_sEntryName);
	theApp.SetIniString(gchMain, gchUserCount,		L2S(m_iUserCount));
	theApp.SetIniString(gchMain, gchCurrentUser,	L2S(m_iCurrentUser));
	theApp.SetIniString(gchMain, gchLastTimeC,		L2S(m_lLastTimeC));
	theApp.SetIniString(gchMain, gchDialOnStart,	L2S(m_bDialOnStart));
	theApp.SetIniString(gchMain, gchAutoReDial,		L2S(m_bAutoReDial));
	theApp.SetIniString(gchMain, gchTimeout,		L2S(m_iTimeout));
	theApp.SetIniString(gchMain, gchMinimizeOnConnect,L2S(m_bMinimizedOnConnect));
	theApp.SetIniString(gchMain, gchHangUpOnExit,	L2S(m_bHangUpOnExit));
	theApp.SetIniString(gchMain, gchTimingDial,		L2S(bIsTimingDial));
	theApp.SetIniString(gchMain, gchDialTime,		strDialTime);
		UpdateData(FALSE);
	return TRUE;
}

//�����û���Ϣ
BOOL CDialHelperDlg::LoadCurrentUserInfo(INT i/*=1*/)
{
	CString sss = _T("User") + L2S(i);
	UpdateData();
	m_sUserName = theApp.GetIniString(sss, gchUserName, _T(""));
	m_sPassword = theApp.GetIniString(sss, gchPassword, _T(""));
	UpdateData(FALSE);
	return TRUE;
}
//�����û���Ϣ
BOOL CDialHelperDlg::SaveUserInfo(INT i, CString un, CString pw)
{
	CString sss = _T("User") + L2S(i);	
	//UpdateData();
	theApp.SetIniString(sss, gchUserName, un);
	theApp.SetIniString(sss, gchPassword, pw);
	//UpdateData(FALSE);
	return TRUE;
}
CString CDialHelperDlg::GetUserName(INT i)
{
	CString sss = _T("User") + L2S(i);	
	return theApp.GetIniString(sss, gchUserName, _T(""));
}
CString CDialHelperDlg::GetPassword(INT i)
{
	CString sss = _T("User") + L2S(i);	
	return theApp.GetIniString(sss, gchPassword, _T(""));
}
//����ȫ���û���Ϣ
BOOL CDialHelperDlg::LoadAllUserInfo()
{
	CB_USERNAME.ResetContent();
	for (INT i = 1; i <= m_iUserCount; i++)
	{
		CB_USERNAME.AddString(GetUserName(i));
		UpdateData(FALSE);
	}
	CB_USERNAME.SetCurSel(m_iCurrentUser - 1);
	UpdateData();
	return TRUE;
}


void CDialHelperDlg::OnBnClickedCheckAutodial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}

void CDialHelperDlg::OnBnClickedCheckRedial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}

void CDialHelperDlg::OnBnClickedCheckOnlineMinsize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}

void CDialHelperDlg::OnBnClickedCheckHangup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}

void CDialHelperDlg::OnBnClickedCheckLog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveAllToIni();
}

void CDialHelperDlg::OnEnChangeEditRedialTime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	long l = m_iTimeout ;
	if (l <= 1)
		m_iTimeout = 1;
	if (l > 70)
		m_iTimeout = 70;
	UpdateData(FALSE);

}

void CDialHelperDlg::OnBnClickedButtonOpenlog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(0, _T("open"), theApp.sLogFileName, NULL, NULL, SW_SHOW);
}

void CDialHelperDlg::OnEnChangeEditPassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	SaveUserInfo(m_iCurrentUser, m_sUserName, m_sPassword);
	UpdateData(FALSE);
}

void CDialHelperDlg::OnBnClickedButtonOption()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����������չ������
	CString str;
	if (GetDlgItemText(IDC_BUTTON_OPTION, str),str == _T("����(&S)>>"))
	{
		SetDlgItemText(IDC_BUTTON_OPTION, _T("����(&S)<<"));
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_OPTION, _T("����(&S)>>"));
	}

	static CRect rectLarge;
	static CRect rectSmall;

	if (rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&rectLarge);
		GetDlgItem(IDC_SEPARATE)->GetWindowRect(&rectSeparator);

		rectSmall.left = rectLarge.left;
		rectSmall.top = rectLarge.top;
		rectSmall.right = rectSeparator.right;
		rectSmall.bottom = rectLarge.bottom;
	}
	if (str == _T("����(&S)<<"))
	{
		SetWindowPos(NULL, 0, 0, rectSmall.Width(), rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rectLarge.Width(), rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CDialHelperDlg::OnBnClickedButtonProvEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RasEditPhonebookEntry(GetSafeHwnd(), NULL, m_sEntryName);
}

//���Ű�ť�Ĵ������
void CDialHelperDlg::OnDial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!bDialing)
	{
		bDialing = TRUE;
		/*theApp.LOG(CString(_T("����:<") + m_sEntryName));*/
	}

	SetButtonsState();
}
//���Ź��̺���
LRESULT CDialHelperDlg::OnDialFunc(WPARAM w, LPARAM l)
{
	TCHAR szMessage[256];

	m_StatusText = GetRasConnState((RASCONNSTATE) w);
	UpdateData(FALSE);
	if (l)  // �������
	{
		if (RasGetErrorString((UINT) l, szMessage, 256) != 0)
		{
			CString sss;
			sss.LoadString(IDS_RASCS_25);

			wsprintf(szMessage, sss);
		}
		CString sss2 = _T("����") + L2S(l) + _T(":");
		sLastError = sss2 + szMessage;
		//TRACE(CString("External error ") + CString(szMessage) + "\n");
		HangUp(FALSE);
		/*theApp.LOG(szMessage);*/
		//OnDialNextPhone();
	}
	else if (RASCS_DONE & w)
	{
		bConnected = TRUE;
		SaveLastTimeC();
		m_StatusText = _T("������.");
		sLastError = _T("");
		if (theApp.bAUTO)
		{
			Sleep(5000);
			this->DestroyWindow();
		}
		/*theApp.LOG(_T("������."));*/
		SetButtonsState();
	}
	UpdateData(FALSE);
	return 0;
}
//�趨��ť״̬
void CDialHelperDlg::SetButtonsState()
{
	CString sss;

	if (bDialing == TRUE)
	{
		GetDlgItem(IDC_DIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_HANGUP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DIAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_HANGUP)->EnableWindow(FALSE);
	}

	// 	m_ProgressTimeout.ShowWindow(SW_HIDE);
	// 	if (m_iTimeout && bDialing && !bConnected)
	// 	{
	// 		m_ProgressTimeout.ShowWindow(SW_SHOW);
	// 	}
	// 	m_ProgressTimeout.SetPos(0);
}
//�Ҷϣ�������TRUEʱ�������˳�
BOOL CDialHelperDlg::HangUp(BOOL bExitApp)
{
	DWORD dwRet;
	RASCONNSTATUS rStatus;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	if ((dwRet != ERROR_INVALID_HANDLE) || (bDialNow))
	{
		bDialNow = FALSE;
		//�Ҷ�����
		RasHangUp(hRasConn);
		dwRet = RasGetConnectStatus(hRasConn, &rStatus);
		m_StatusText = _T("�Ͽ����ӡ�");
		UpdateData(FALSE);
		//��õ�ǰʱ��
		CTime t1 = CTime::GetCurrentTime();
		CTimeSpan ts;
		MSG msg;
		while (dwRet != ERROR_INVALID_HANDLE)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			dwRet = RasGetConnectStatus(hRasConn, &rStatus);
			ts = CTime::GetCurrentTime() - t1;
			if (!bExitApp)
				if (ts.GetTotalSeconds() > 3)
					break;
		}
		//�趨��־
		/*theApp.LOG(_T("�Ҷ�."));*/
		m_StatusText = _T("��ͣ...");
		UpdateData(FALSE);
	}
	Sleep(1000);
	m_StatusText.Empty();
	UpdateData(FALSE);
	bConnected = FALSE;
	return TRUE;
}
//������ӵĵ�ǰ״̬
CString CDialHelperDlg::GetRasConnState(RASCONNSTATE rasconn)
{
	CString sss;
	switch (rasconn)
	{
	case RASCS_OpenPort:
		sss.LoadString(IDS_RASCS_1); break;
	case RASCS_PortOpened:
		sss.LoadString(IDS_RASCS_2); break;
	case RASCS_ConnectDevice:
		sss.LoadString(IDS_RASCS_3); break;
	case RASCS_DeviceConnected:
		sss.LoadString(IDS_RASCS_4); break;
	case RASCS_AllDevicesConnected:
		sss.LoadString(IDS_RASCS_5); break;
	case RASCS_Authenticate:
		sss.LoadString(IDS_RASCS_6); break;
	case RASCS_AuthNotify:
		sss.LoadString(IDS_RASCS_7); break;
	case RASCS_AuthRetry:
		sss.LoadString(IDS_RASCS_8); break;
	case RASCS_AuthCallback:
		sss.LoadString(IDS_RASCS_9); break;
	case RASCS_AuthChangePassword:
		sss.LoadString(IDS_RASCS_10); break;
	case RASCS_AuthProject:
		sss.LoadString(IDS_RASCS_11); break;
	case RASCS_AuthLinkSpeed:
		sss.LoadString(IDS_RASCS_12); break;
	case RASCS_AuthAck:
		sss.LoadString(IDS_RASCS_13); break;
	case RASCS_ReAuthenticate:
		sss.LoadString(IDS_RASCS_14); break;
	case RASCS_Authenticated:
		sss.LoadString(IDS_RASCS_15); break;
	case RASCS_PrepareForCallback:
		sss.LoadString(IDS_RASCS_16); break;
	case RASCS_WaitForModemReset:
		sss.LoadString(IDS_RASCS_17); break;
	case RASCS_WaitForCallback:
		sss.LoadString(IDS_RASCS_18); break;
	case RASCS_Interactive:
		sss.LoadString(IDS_RASCS_19); break;
	case RASCS_RetryAuthentication:
		sss.LoadString(IDS_RASCS_20); break;
	case RASCS_CallbackSetByCaller:
		sss.LoadString(IDS_RASCS_21); break;
	case RASCS_PasswordExpired:
		sss.LoadString(IDS_RASCS_22); break;
	case RASCS_Connected:
		sss.LoadString(IDS_RASCS_23); break;
	case RASCS_Disconnected:
		sss.LoadString(IDS_RASCS_24); break;
	default:
		sss.LoadString(IDS_RASCS_25); break;
	}
	return sss;
}
//Timer�¼�����ѭ��
void CDialHelperDlg::OnTimer(UINT_PTR nIDEvent)
{
	// �����ʱ���Ѿ��򿪣��򷵻�
	switch (nIDEvent)
	{
	case 1:
		if (bInOnTimer)
			return;
		//�����趨ΪTRUE
		bInOnTimer = TRUE;
		// �ж��Ƿ����ڲ���
		if (!bDialing)
		{
			if ((!bDialNow) && (!bConnected))
			{
				CheckExternalConnect();
			}
			bInOnTimer = FALSE;
			return;
		}
		// �ж��Ƿ��Ѿ�������
		if (bConnected)
		{
			CheckingConnected();
			bInOnTimer = FALSE;
			return;
		}
		if (bDialNow)
			CheckingDialing();
		else
			// ��ʼ��?
			StartDial();
		//��ʱ��������ΪFALSE
		bInOnTimer = FALSE;
		break;
	case 2:
		if (bIsTimingDial)
		{
			COleDateTime cdtNowTime;
			CString sTemp;
			cdtNowTime=COleDateTime::GetCurrentTime();
			sTemp=cdtNowTime.Format(__TIME_FORMAT);
// 			SetDlgItemText(IDC_LastError,sTemp);
// 			SetDlgItemText(IDC_STATUS,strDialTime);
			if (sTemp==strDialTime)
			{
				OnDial();
			}

		}
	}

}
//��ʼ����,��OnTimer����
BOOL CDialHelperDlg::StartDial()
{
	bDialNow = TRUE;
	hRasConn = NULL;

	tBeginDial = CTime::GetCurrentTime();
	RASDIALPARAMS rdParams;
	TCHAR szBuf[256] = _T("");
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);
	CString un, pw;
	CXKUsername xk(m_sUserName, 18,m_lLastTimeC);
	m_lLastTimeC = xk.GetLastTimeC();
	un = xk.Realusername();
	TRACE(un);
	TRACE("\n");
	pw = m_sPassword;
	lstrcpy(rdParams.szEntryName, m_sEntryName);
	lstrcpy(rdParams.szUserName, un);
	lstrcpy(rdParams.szPassword, pw);
	rdParams.szCallbackNumber[0] = _T('*');
	rdParams.szDomain[0] = _T('*');
	m_StatusText = _T("������...");
	//m_StatusText = _T("����:<") + m_sEntryName +_T("> �û���:<") + un + _T("> ����:<") + m_sPassword + _T(">");
	/*theApp.LOG(m_StatusText);*/

	UpdateData(FALSE);

	Sleep(400);
	DWORD dwRet;
	//TRACE("DialNow...\n");
	dwRet = RasDial(NULL, NULL, &rdParams, 0L, (LPVOID) RasDialFunc, &hRasConn);
	if (dwRet)
	{
		CString ss;
		ss.Format(_T("%lu"), dwRet);
		//TRACE("dwRet...\n");
		//TRACE(ss);
		//TRACE("\n");
		if (RasGetErrorString((UINT) dwRet, szBuf, 256) != 0)
			m_StatusText = szBuf;
		sLastError = szBuf;
		/*theApp.LOG(m_StatusText);*/
		UpdateData(FALSE);
		HangUp(FALSE);
	}
	return TRUE;
}


//���ָ�����Ƶ������Ƿ����
BOOL CDialHelperDlg::QueryConnections(CString sEntryName)
{
	DWORD cbBuf;
	DWORD cEntry;
	UINT ndx;
	TCHAR szMessage[256];

	DWORD dwRet;
	LPBYTE lpBuffer;

	LPRASENTRYNAME lpRasEntry = new RASENTRYNAME;
	lpRasEntry->dwSize = sizeof(RASENTRYNAME);
	cbBuf = sizeof(RASENTRYNAME);
	//�����������
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);
	delete lpRasEntry;

	lpBuffer = new BYTE[cbBuf];
	lpRasEntry = (LPRASENTRYNAME) lpBuffer;
	lpRasEntry->dwSize = sizeof(RASENTRYNAME);
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);


	if (ERROR_BUFFER_TOO_SMALL == dwRet)
	{
		delete lpRasEntry;
		lpRasEntry = NULL;
		lpRasEntry = (LPRASENTRYNAME) new TCHAR[cbBuf];
		if (NULL != lpRasEntry)
		{
			ZeroMemory(lpRasEntry, cbBuf);
			lpRasEntry->dwSize = cbBuf;
			dwRet = RasEnumEntries(NULL, NULL, lpRasEntry, &cbBuf, &cEntry);
		}
		else
			dwRet = ERROR_NOT_ENOUGH_MEMORY;
	}
	if (0 != dwRet) // ������?
	{
		if (0 != RasGetErrorString((UINT) dwRet, (LPTSTR) szMessage, 256))
		{
			CString sss; sss.LoadString(IDS_RASCS_25);
			wsprintf((LPTSTR) szMessage, sss);
		}
		m_StatusText = szMessage;
		UpdateData(FALSE);
	}
	if (0 == cEntry)
	{
		delete lpRasEntry ;
		return FALSE;
	}
	BOOL bSet = FALSE;
	//����ָ�����������Ƿ����
	if (0 == dwRet)  // û�д�?
	{
		for (ndx = 0; ndx < cEntry; ndx++)
		{
			if (sEntryName == lpRasEntry[ndx].szEntryName)
			{
				delete lpRasEntry ;
				return TRUE;
			}
		}
	}
	delete lpRasEntry ;
	return FALSE;
}
//����Ƿ�������
BOOL CDialHelperDlg::CheckingConnected()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (ERROR_INVALID_HANDLE == dwRet)
	{
		HangUp(FALSE);
		m_StatusText = _T("���ӱ��Ͽ���");
		/*theApp.LOG(_T("Lost of carrier."));*/
		UpdateData(FALSE);

		if (!m_bAutoReDial)
		{
			OnDial();
		}
		/*TRACE("RedialWhenHangUp = FALSE\n");*/
		return TRUE;
	}

	return TRUE;
}
//���ż��
BOOL CDialHelperDlg::CheckingDialing()
{
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	dwRet = RasGetConnectStatus(hRasConn, &rStatus);
	RASCONNSTATE rcs = rStatus.rasconnstate;
	if (!bConnected)
	{
		BOOL m_bMaxWaitTime = m_bAutoReDial;
		if (m_bMaxWaitTime)
		{
			CTimeSpan tst = CTime::GetCurrentTime() - tBeginDial;
			/*m_ProgressTimeout.SetPos(100 * tst.GetTotalSeconds() / m_iTimeout);*/
			if (tst.GetTotalSeconds() > m_iTimeout)
			{
				m_StatusText = _T("���ӳ�ʱ��");
				/*theApp.LOG(_T("Time out."));*/
				UpdateData(FALSE);
				HangUp(FALSE);
			}
		}
	}
	return TRUE;
}

//������������Ƿ�������
BOOL CDialHelperDlg::CheckExternalConnect()
{
	CString s1, s2;
	if (!GetActiveConnectionParam(s1, s2))
		return FALSE;
	if (s1.IsEmpty())
		return FALSE;
	bDialing = TRUE;
	bDialNow = TRUE;
	bConnected = TRUE;
	QueryConnections();

	SetButtonsState();
	m_StatusText = _T("����������");

	UpdateData(FALSE);
	return TRUE;
}
//�õ��Ѿ����ߵ�����
BOOL CDialHelperDlg::GetActiveConnectionParam(CString& Conn, CString& Phon)
{
	RASCONN rcArray[100];
	RASCONNSTATUS rStatus;
	DWORD dwRet;
	DWORD dw1 = sizeof(rcArray);
	DWORD dw2;
	ZeroMemory(rcArray, dw1);
	rcArray[0].dwSize = sizeof(RASCONN);
	dwRet = RasEnumConnections(rcArray, &dw1, &dw2);
	//	ERROR_BUFFER_TOO_SMALL 
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	for (dw1 = 0; dw1 < dw2; dw1++)
	{
		dwRet = RasGetConnectStatus(rcArray[dw1].hrasconn, &rStatus);
		if (RASCS_Connected == rStatus.rasconnstate)
		{
			Conn = rcArray[dw1].szEntryName;
			hRasConn = rcArray[dw1].hrasconn;
			return TRUE;
		}
	}
	return FALSE;
}
//����������
DWORD CDialHelperDlg::CreatEntry(CString sEntryName, CString sUserName,
									CString sPassword)
{
	CString sDeviceName = _T("WAN ΢�Ͷ˿� (PPPOE)");
	CString sDeviceType = _T("");

	RASENTRY re;;
	ZeroMemory(&re, sizeof(re));
	/////////////////////////////////////////////////////////////////////////////////////	
	re.dwSize = sizeof(RASENTRY);
	re.dwCountryCode = 86;
	re.dwCountryID = 86;
	re.dwDialExtraPercent = 75;
	re.dwDialExtraSampleSeconds = 60;
	re.dwDialMode = RASEDM_DialAll;   //1
	re.dwEncryptionType = ET_Optional;
	re.dwfNetProtocols = RASNP_Ip;
	re.dwfOptions = RASEO_RemoteDefaultGateway |
		RASEO_ModemLights |
		RASEO_SwCompression |
		RASEO_SecureLocalFiles |
		RASEO_PreviewUserPw |
		RASEO_ShowDialingProgress;
	re.dwfOptions2 = RASEO2_SecureFileAndPrint |
		RASEO2_SecureClientForMSNet |
		RASEO2_DontNegotiateMultilink |
		RASEO2_DontUseRasCredentials |
		RASEO2_Internet |
		RASEO2_DisableNbtOverIP;
	//RASEO2_ReconnectIfDropped    
	re.dwFramingProtocol = 1;
	re.dwHangUpExtraPercent = 10;
	re.dwHangUpExtraSampleSeconds = 120;
	re.dwRedialCount = 3;
	re.dwRedialPause = 120;
	re.dwType = 5;	
	lstrcpy(re.szDeviceType, RASDT_PPPoE);
	lstrcpy(re.szDeviceName, sDeviceName);

	/////////////////////////////////////////////////////////////////////////////////////

	RASCREDENTIALS rc;
	ZeroMemory(&rc, sizeof(RASCREDENTIALS));
	rc.dwSize = sizeof(RASCREDENTIALS);
	rc.dwMask = RASCM_UserName | RASCM_Password | RASCM_DefaultCreds;

	lstrcpy(rc.szUserName, sUserName);
	lstrcpy(rc.szPassword, sPassword);

	if (RasSetEntryProperties(NULL, sEntryName, &re, sizeof(re), 0, 0) == 0)
	{
		return(RasSetCredentials(NULL, sEntryName, &rc, 0));
	}
	return 0;
}

void CDialHelperDlg::OnDestroy()
{
	KillTimer(uTimer);
	CoUninitialize();
	if (m_bHangUpOnExit)
		HangUp(TRUE);

	CDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
}

void CDialHelperDlg::OnCbnSelchangeCboUsername()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCurrentUser = CB_USERNAME.GetCurSel() + 1;
	LoadCurrentUserInfo(m_iCurrentUser);
}

void CDialHelperDlg::OnBnClickedButtonUsercontrol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUserControl cuc;
	cuc.DoModal();
	LoadAllUserInfo();
}

void CDialHelperDlg::OnBnClickedHangup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (bDialing)
	{
		//TRACE("OnHangup()\n");
		/*theApp.LOG(_T("�Ҷϱ�����."));*/
		bDialing = FALSE;
		HangUp(FALSE);
	}
	SetButtonsState();
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//OnOK();
	CDialog::OnLButtonDown(nFlags, point);
}

void CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnOK();
	CDialog::OnLButtonUp(nFlags, point);
}

void CDialHelperDlg::OnBnClickedButtonAbout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg* abd = new CAboutDlg;
	abd->DoModal();
}

void CDialHelperDlg::OnBnClickedbtnautoshortcut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (CShortcut::Exist(CSIDL_COMMON_STARTUP, _T("�����������.lnk")))
	{
		if (CShortcut::Remove(CSIDL_COMMON_STARTUP, _T("�����������.lnk")))
		{
			SetDlgItemText(btnAutoCreat, _T("����������"));
		}
		else
		{
			MessageBox(_T("δ֪����!"), NULL, MB_ICONSTOP);
		}
	}
	else
	{
		if (CShortcut::Create(CSIDL_COMMON_STARTUP, _T("�����������.lnk"),_T("/auto")))
		{
			SetDlgItemText(btnAutoCreat, _T("ɾ��������"));
		}
		else
		{
			MessageBox(_T("δ֪����!"), NULL, MB_ICONSTOP);
		}
	}
}

void CDialHelperDlg::OnBnClickedchktimeingdial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (bIsTimingDial)
	{
		uTimer2=SetTimer(2,1000,NULL);
	}
	else
	{
		KillTimer(2);
	}

}

void CDialHelperDlg::OnDtnDatetimechangedtdialtime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UpdateData();
	strDialTime=codtDialTime.Format(__TIME_FORMAT);
	
}

void CDialHelperDlg::OnBnClickedbtnsaveini()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	SaveAllToIni();
}

void CDialHelperDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LoadAllFromIni();
	UpdateData(FALSE);
}
