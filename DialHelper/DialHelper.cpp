// DialHelper.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DialHelper.h"
#include "DialHelperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialHelperApp

BEGIN_MESSAGE_MAP(CDialHelperApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDialHelperApp ����

CDialHelperApp::CDialHelperApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDialHelperApp ����

CDialHelperApp theApp;


// CDialHelperApp ��ʼ��

BOOL CDialHelperApp::InitInstance()
{
	AfxOleInit();
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//�Ӵ˿�ʼΪ���Ƴ���࿪------------
	CWnd* pWndPrev;
	CWnd* pWndChild;
	//���Ҵ���
	pWndPrev = CWnd::FindWindow(_T("#32770"), CAPTION_NAME);
	if (pWndPrev)
	{
		pWndChild = pWndPrev->GetLastActivePopup();

		if (pWndPrev->IsIconic())
			pWndPrev->ShowWindow(SW_RESTORE);
		pWndChild->SetForegroundWindow();
		return FALSE;
	}
	//��Ϊ���Ƴ���࿪-�Ľ�������-----------
	//��ȡ�����в���
	CString aaa = m_lpCmdLine;
	aaa.MakeUpper();
	if (aaa.Find(_T("/AUTO")) != -1)
	{
		bAUTO = TRUE;
	}
	else
	{
		bAUTO = FALSE;
	}

	//���������·�����ļ���
	GetModuleFileName(NULL, sExePath.GetBuffer(MAX_PATH), MAX_PATH);
	//�ͷſռ�
	sExePath.ReleaseBuffer();

	sExePath.MakeReverse();
	sExePath = sExePath.Right(sExePath.GetLength() - sExePath.Find(_T('\\')));
	sExePath.MakeReverse();
	// 	TRACE(sIniFileName);
	// 	TRACE("\n");
	sIniFileName = sExePath + _T("NetKeeper.ini");
	sLogFileName = sExePath + _T("NetKeeper.log");

	pDlg = new CDialHelperDlg;
	m_pMainWnd = pDlg;
	INT_PTR nResponse = pDlg->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

CString CDialHelperApp::GetIniString(CString sec, CString key, CString str)
{
	CString sss;
	GetPrivateProfileString(sec, key, str, sss.GetBuffer(512), 511,
		sIniFileName);
	sss.ReleaseBuffer();
	return sss;
}

BOOL CDialHelperApp::SetIniString(CString sec, CString key, CString str)
{
	return WritePrivateProfileString(sec, key, str, sIniFileName);
}
// BOOL CDialHelperApp::LOG(UINT nID)
// {
// 	CString sss;
// 	if (!sss.LoadString(nID)) return FALSE;
// 	LOG(sss);
// 	return TRUE;
// }
// BOOL CDialHelperApp::LOG(CString sss)
// {
// 	TRY
// 	{
// 		CFile f(sLogFileName,
// 			CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone);
// 		TRY
// 		{
// 			//���ļ������
// 			f.SeekToEnd();
// 			//�õ���ǰ��ʱ��
// 			CTime t = CTime::GetCurrentTime();
// 			CString aaa;
// 			//����һ�������ڸ�ʽ���
// 			wsprintf(aaa.GetBuffer(512),
// 				_T("%4d-%02d-%02d %02d:%02d:%02d %s\r\n"),
// 				t.GetYear(), t.GetMonth(), t.GetDay(),
// 				t.GetHour(), t.GetMinute(), t.GetSecond(), sss);
// 			aaa.ReleaseBuffer();
// 			f.Write(aaa, aaa.GetLength());
// 			f.Close();
// 		}
// 		CATCH(CFileException, e)
// 		{
// 			//�ر��ļ�
// 			f.Close();
// 		}
// 		END_CATCH
// 	}
// 	CATCH(CFileException, e)
// 	{
// 		TRACE(CString(_T("�ļ����ܱ���") + e->m_cause) + _T("\n"));
// 		return FALSE;
// 	}
// 	END_CATCH
// 		return TRUE;
// }

VOID CDialHelperApp::PostDialMessage(UINT unMsg, WPARAM rasconnstate,
	DWORD dwError)
{
	pDlg->PostMessage(WM_DIAL_FUNC, rasconnstate, dwError);
}
