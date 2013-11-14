// CreatUserInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialHelper.h"
#include "CreatUserInfo.h"


// CCreatUserInfo �Ի���

IMPLEMENT_DYNAMIC(CCreatUserInfo, CDialog)

// CCreatUserInfo::CCreatUserInfo(CWnd* pParent /*=NULL*/)
// : CDialog(CCreatUserInfo::IDD, pParent)
// , m_sUserName(_T(""))
// , m_sPassword(_T(""))
// {
// 	EnableAutomation();
// }
CCreatUserInfo::CCreatUserInfo(INT i, CWnd* pParent /*=NULL*/) : CDialog(CCreatUserInfo::IDD, pParent),
	m_sUserName(_T("")), m_sPassword(_T(""))
{
	m_iCurrentUser = i;
	EnableAutomation();
}

CCreatUserInfo::~CCreatUserInfo()
{
}

void CCreatUserInfo::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDialog::OnFinalRelease();
}

void CCreatUserInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_sUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_sPassword);
	DDV_MaxChars(pDX, m_sPassword, 20);
	DDV_MaxChars(pDX, m_sUserName, 20);
}

BOOL CCreatUserInfo::OnInitDialog()
{
	m_sUserName = theApp.pDlg->GetUserName(m_iCurrentUser);
	m_sPassword = theApp.pDlg->GetPassword(m_iCurrentUser);
	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCreatUserInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CCreatUserInfo::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCreatUserInfo, CDialog)
END_DISPATCH_MAP()

// ע��: ������� IID_ICreatUserInfo ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {A23F3EEB-7E27-4995-AF1F-3DFE8EF4FE65}
static const IID IID_ICreatUserInfo =
{
	0xA23F3EEB, 0x7E27, 0x4995,
	{ 0xAF, 0x1F, 0x3D, 0xFE, 0x8E, 0xF4, 0xFE, 0x65 }
};

BEGIN_INTERFACE_MAP(CCreatUserInfo, CDialog)
INTERFACE_PART(CCreatUserInfo, IID_ICreatUserInfo, Dispatch)
END_INTERFACE_MAP()


// CCreatUserInfo ��Ϣ�������

void CCreatUserInfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_sUserName.IsEmpty() || m_sPassword.IsEmpty())
	{
		MessageBox(_T("����д�û��������룡"), _T("��ʾ"));
	}
	else
	{
		if (m_iCurrentUser > theApp.pDlg->m_iUserCount)
		{
			(theApp.pDlg->m_iUserCount)++;
			theApp.pDlg->SaveAllToIni();
		} 
		theApp.pDlg->SaveUserInfo(m_iCurrentUser, m_sUserName, m_sPassword);
		OnOK();
	}
}
