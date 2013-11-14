// DialHelperDlg.h : ͷ�ļ�
//

#pragma once
#pragma comment(lib, "Rasapi32.lib") 
#include <ras.h>
#include <raserror.h>
#include <RasDlg.h>
#include "afxcmn.h"
#include "afxwin.h"




// CDialHelperDlg �Ի���
class CDialHelperDlg : public CDialog
{
	// ����
public:
	CDialHelperDlg(CWnd* pParent = NULL);	// ��׼���캯��

	//�Լ�����ĺ���-------------------------------------------------------------------------------------

	BOOL bDialing;			//�Ƿ���������
	BOOL bDialNow;			//�Ƿ�Ҫ����
	BOOL bConnected;		//�Ƿ�������
	BOOL bInOnTimer;		//�Ƿ������˶�ʱ��
	CTime tBeginDial;		//��ʼ����ʱ�� ����ʱ����
	UINT_PTR uTimer;		//��ʱ��
	UINT_PTR uTimer2;
	BOOL bInOnTimer2;		//�Ƿ������˶�ʱ��

//	CTrayIcon cti;

	RASENTRYNAME m_renEntries[20];
	ULONG m_ulEntryCount;

	BOOL SaveAllToIni();	//���ô洢
	BOOL LoadAllFromIni();	//���ö�ȡ
	BOOL SaveLastTimeC();	//�洢LastTimeC
	BOOL LoadCurrentUserInfo(INT i = 1);	//���뵱ǰ�û���Ϣ
	BOOL SaveUserInfo(INT i = 1, CString un = _T(""), CString pw = _T(""));	//���浱ǰ�û���Ϣ
	BOOL LoadAllUserInfo();
	CString GetUserName(INT i);
	CString GetPassword(INT i);

	BOOL StartDial();
	BOOL HangUp(BOOL bExitApp);	//�Ҷ�

	BOOL QueryConnections(CString sEntryName = _T(""));
	CString GetRasConnState(RASCONNSTATE rasconn);
	BOOL CheckingConnected();
	BOOL CheckingDialing();
	BOOL CheckExternalConnect();
	BOOL GetActiveConnectionParam(CString& Conn, CString& Phon);


	void SetButtonsState();	//���ð�ť״̬
	//�Լ�����ĺ���-��������------------------------------------------------------------------------------------

	// �Ի�������
	enum
	{
		IDD = IDD_DialHelper_DIALOG
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString m_sEntryName;
	CString m_sUserName;
	CString m_sPassword;

	INT m_iUserCount;
	INT m_iCurrentUser;
	LONG m_lLastTimeC;
	BOOL m_bDialOnStart;
	BOOL m_bAutoReDial;
	INT m_iTimeout;
	BOOL m_bMinimizedOnStart;
	BOOL m_bMinimizedOnConnect;
	BOOL m_bHangUpOnExit;
// 	BOOL m_bEnableLog;
// 	CString m_sLogFile;
	


	CString m_StatusText;

public:
	afx_msg void OnBnClickedCheckAutodial();
	afx_msg void OnBnClickedCheckRedial();
	afx_msg void OnBnClickedCheckOnlineMinsize();
	afx_msg void OnBnClickedCheckHangup();
	afx_msg void OnBnClickedCheckLog();
	afx_msg void OnEnChangeEditRedialTime();
	afx_msg void OnBnClickedButtonOpenlog();
	afx_msg void OnEnChangeEditPassword();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnBnClickedButtonProvEdit();
	afx_msg void OnDial();
	afx_msg LRESULT OnDialFunc(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	/*CProgressCtrl m_ProgressTimeout;*/
public:
	DWORD CreatEntry(CString sEntryName, CString sUserName = _T(""),
		CString sPassword = _T(""));
public:
	afx_msg void OnDestroy();
public:
	CComboBox CB_USERNAME;
public:
	afx_msg void OnCbnSelchangeCboUsername();

public:
	afx_msg void OnBnClickedButtonUsercontrol();
public:
	CString sText;
public:
	afx_msg void OnBnClickedHangup();
public:
	afx_msg void OnBnClickedButtonAbout();
public:
	CString sLastError;
public:
	afx_msg void OnBnClickedbtnautoshortcut();
public:
	BOOL bIsTimingDial;
public:
	afx_msg void OnBnClickedchktimeingdial();
public:
	afx_msg void OnDtnDatetimechangedtdialtime(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedbtnsaveini();
public:
	COleDateTime codtDialTime;
public:
	CString strDialTime;
public:
	afx_msg void OnBnClickedButton1();
};
