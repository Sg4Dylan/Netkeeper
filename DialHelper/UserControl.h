#pragma once
#include "afxcmn.h"


// CUserControl �Ի���

class CUserControl : public CDialog
{
	DECLARE_DYNAMIC(CUserControl)

public:
	CUserControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserControl();


	virtual void OnFinalRelease();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOG_USERCONTROL
	};
protected:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CListCtrl CLC_UserInfo;
	BOOL ReloadUserInfo();
	BOOL InitUserInfo();
public:
	afx_msg void OnBnClickedButtonNew();
public:
	afx_msg void OnBnClickedButtonChange();
public:
	afx_msg void OnBnClickedButtonDelete();
};
