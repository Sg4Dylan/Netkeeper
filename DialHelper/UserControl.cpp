// UserControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialHelper.h"
#include "UserControl.h"
#include "CreatUserInfo.h"


// CUserControl �Ի���

IMPLEMENT_DYNAMIC(CUserControl, CDialog)

CUserControl::CUserControl(CWnd* pParent /*=NULL*/) : CDialog(CUserControl::IDD, pParent)
{
	EnableAutomation();
}

CUserControl::~CUserControl()
{
}

void CUserControl::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDialog::OnFinalRelease();
}

void CUserControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, CLC_UserInfo);
}

BOOL CUserControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUserInfo();
	ReloadUserInfo();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CUserControl, CDialog)
	ON_BN_CLICKED(ID_BUTTON_NEW, &CUserControl::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CUserControl::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CUserControl::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUserControl, CDialog)
END_DISPATCH_MAP()

// ע��: ������� IID_IUserControl ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {21490940-CCD4-4738-B553-87D2DDE3D370}
static const IID IID_IUserControl =
{
	0x21490940, 0xCCD4, 0x4738,
	{ 0xB5, 0x53, 0x87, 0xD2, 0xDD, 0xE3, 0xD3, 0x70 }
};

BEGIN_INTERFACE_MAP(CUserControl, CDialog)
INTERFACE_PART(CUserControl, IID_IUserControl, Dispatch)
END_INTERFACE_MAP()

BOOL CUserControl::ReloadUserInfo()
{
	CLC_UserInfo.DeleteAllItems();
	for (int i = 1; i <= theApp.pDlg->m_iUserCount; i++)
	{
		CLC_UserInfo.InsertItem(i - 1, L2S(i));
		CLC_UserInfo.SetItemText(i - 1, 1, theApp.pDlg->GetUserName(i));
		CLC_UserInfo.SetItemText(i - 1, 2, theApp.pDlg->GetPassword(i));
	}
	UpdateData();
	return TRUE;
}

BOOL CUserControl::InitUserInfo()
{
	CLC_UserInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, 40);
	CLC_UserInfo.InsertColumn(1, _T("�û���"), LVCFMT_LEFT, 150);
	CLC_UserInfo.InsertColumn(2, _T("����"), LVCFMT_LEFT, 120);
	CLC_UserInfo.SetExtendedStyle(CLC_UserInfo.GetExtendedStyle() |
					LVS_EX_FULLROWSELECT) ;
	return TRUE;
}
// CUserControl ��Ϣ�������

void CUserControl::OnBnClickedButtonNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCreatUserInfo* cui = new CCreatUserInfo(theApp.pDlg->m_iUserCount + 1,
								NULL);
	cui->DoModal();
	ReloadUserInfo();
}

void CUserControl::OnBnClickedButtonChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CLC_UserInfo.GetSelectionMark() == -1)
	{
		MessageBox(_T("��ѡ��Ҫ�޸ĵ��"), _T("��ʾ"));
	}
	else
	{
		CCreatUserInfo* cui = new CCreatUserInfo(CLC_UserInfo.GetSelectionMark() +
									1);
		cui->DoModal();
		ReloadUserInfo();
	}
}

void CUserControl::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CLC_UserInfo.GetSelectionMark() == -1)
	{
		MessageBox(_T("��ѡ��Ҫɾ�����"), _T("��ʾ"));
	}
	else
	{
		for (INT i = CLC_UserInfo.GetSelectionMark() + 1;
			i < theApp.pDlg->m_iUserCount;
			i++)
		{
			theApp.pDlg->SaveUserInfo(i, theApp.pDlg->GetUserName(i + 1),
							theApp.pDlg->GetPassword(i + 1));
		}

		CString sss = _T("User") + L2S(theApp.pDlg->m_iUserCount);
		theApp.SetIniString(sss, gchUserName, NULL);
		theApp.SetIniString(sss, gchPassword, NULL);
		//�߽��ж���m_iUserCount������ڵ���1
		if (theApp.pDlg->m_iUserCount > 1)
		{
			(theApp.pDlg->m_iUserCount)--;
		}
		//�߽��ж���m_iCurrentUser����С�ڵ���m_iUserCount
		if (theApp.pDlg->m_iCurrentUser > theApp.pDlg->m_iUserCount)
		{
			theApp.pDlg->m_iCurrentUser = theApp.pDlg->m_iUserCount;
		}

		theApp.pDlg->SaveAllToIni();
		ReloadUserInfo();
	}
}
