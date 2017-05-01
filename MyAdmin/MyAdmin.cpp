
// MyAdmin.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyAdmin.h"
#include "MainFrm.h"

#include "MyAdminDoc.h"
#include "MyAdminView.h"
#include "cfg_op.h"
#include "DlgInitCfg.h"
#include "secmng_globvar.h"



//  hpp		.h .cpp
// secmng_globvar.h

//1 ��֤ȫ�ֱ��� ��������ֻ����һ�� �����ڴ�һ��
//2 ͷ�ļ���ͬһ��cpp�б���������  ��  ͷ�ļ�������ط����� ��������ͬ������

#ifndef SECMNG_GLOB_VAR_H_
#define SECMNG_GLOB_VAR_H_

#include"stdafx.h"

//����Դȫ�ֱ���
CString	g_dbSource, g_dbUse, g_dbpw;

// ����10������ ��Ӧ�����ʵ��
//  
CDatabase	myDB;
CDatabase	*g_pDB = NULL;

CString		g_SecMngIniName = "secmngadmin.ini";


#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyAdminApp

BEGIN_MESSAGE_MAP(CMyAdminApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMyAdminApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMyAdminApp ����

CMyAdminApp::CMyAdminApp()
{
	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyAdmin.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMyAdminApp ����

CMyAdminApp theApp;


// CMyAdminApp ��ʼ��
int CMyAdminApp::ReadConfig()
{
	int						ret = 0;
	char					strFileName[4096];
	int						strFileNameLen = 4096;

	//�������ļ� ��������ļ���·�� 
	GetModuleFileName(AfxGetInstanceHandle(), strFileName, strFileNameLen);
	
	CString strFilePath = strFileName;
	int i = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(i);
	
	strFilePath = strFilePath + '\\' + "secmngadmin.ini";
	//AfxMessageBox(strFilePath);

	char tmpBuf[4096];
	int tmpLen = 4096;

	//�������ļ�
	//DSN
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath , "DSN", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("�������� DSN ʧ��");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbSource = tmpBuf;  //��ȫ�ֱ�������Դ

	//UID 
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "UID", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("�������� UID ʧ��");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbUse = tmpBuf;

	//PWD  
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "PWD ", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("�������� PWD  ʧ��");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbpw = tmpBuf;

	
	return ret;
}

int CMyAdminApp::ConnectDBByodbc()
{
	int		iTagDb = 0; //0 �ɹ�   1 �쳣

	CString		strCon;


	//CDatabase  mydb[20];

	//mydb->OpenEx()

	strCon.Format("DSN=%s;UID=%s;PWD=%s", g_dbSource, g_dbUse, g_dbpw);
	TRY
	{

		//�����ݿ�
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("ͨ������Դ�������ݿ�ʧ��");
			return -1;
		}
		//ϵͳ������ʱ�� ������һ������ (������)
	}
	CATCH_ALL(e)
	{
		iTagDb = 1;
		e->ReportError();
	}
	END_CATCH_ALL

	if (iTagDb == 1)
	{
		AfxMessageBox("ͨ������Դ�������ݿ� err");
		return -1;
	}
	else
	{
		//AfxMessageBox("ͨ������Դ�������ݿ� ok");
	}
	g_pDB = &myDB;
	return 0;
}

BOOL CMyAdminApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	//�������ļ�
	int			ret = 0;
	ret = ReadConfig();
	if (ret != 0)
	{
		CDlgInitCfg  dlgInitCfg;
		if (dlgInitCfg.DoModal() == IDCANCEL)
		{
			AfxMessageBox("�������ļ�ʧ��");
			return FALSE;
		}
		else
		{
			 g_dbSource = dlgInitCfg.m_strDbSource;
			 g_dbUse = dlgInitCfg.m_strDbUser;
			 g_dbpw = dlgInitCfg.m_strDbPw;
		}
	}
	//�������ݿ�
	ret = ConnectDBByodbc();
	if (ret != 0)
	{
		AfxMessageBox("ͨ������Դ�������ݿ� err");
		return FALSE;
	}
	//AfxMessageBox("ͨ������Դ�������ݿ� ok");
	



	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMyAdminView));
	if (!pDocTemplate)
		return FALSE;
	this->AddDocTemplate(pDocTemplate);
	


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	//m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CMyAdminApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMyAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyAdminApp ��Ϣ�������



