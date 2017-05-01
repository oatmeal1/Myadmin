
// MyAdmin.cpp : 定义应用程序的类行为。
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

//1 保证全局变量 被定义且只定义一次 分配内存一次
//2 头文件在同一个cpp中被包含两次  和  头文件被多个地方包含 是两个不同的事情

#ifndef SECMNG_GLOB_VAR_H_
#define SECMNG_GLOB_VAR_H_

#include"stdafx.h"

//数据源全局变量
CString	g_dbSource, g_dbUse, g_dbpw;

// 建立10条连接 你应该如何实现
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMyAdminApp 构造

CMyAdminApp::CMyAdminApp()
{
	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyAdmin.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMyAdminApp 对象

CMyAdminApp theApp;


// CMyAdminApp 初始化
int CMyAdminApp::ReadConfig()
{
	int						ret = 0;
	char					strFileName[4096];
	int						strFileNameLen = 4096;

	//读配置文件 求出配置文件的路径 
	GetModuleFileName(AfxGetInstanceHandle(), strFileName, strFileNameLen);
	
	CString strFilePath = strFileName;
	int i = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(i);
	
	strFilePath = strFilePath + '\\' + "secmngadmin.ini";
	//AfxMessageBox(strFilePath);

	char tmpBuf[4096];
	int tmpLen = 4096;

	//读配置文件
	//DSN
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath , "DSN", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置项 DSN 失败");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbSource = tmpBuf;  //给全局变量数据源

	//UID 
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "UID", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置项 UID 失败");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbUse = tmpBuf;

	//PWD  
	memset(tmpBuf, 0, sizeof(tmpBuf));
	ret = GetCfgItem((LPTSTR)(LPCTSTR)strFilePath, "PWD ", tmpBuf, &tmpLen);
	if (ret != 0)
	{
		AfxMessageBox("读配置项 PWD  失败");
		return -1;
	}
	tmpBuf[tmpLen] = '\0';
	g_dbpw = tmpBuf;

	
	return ret;
}

int CMyAdminApp::ConnectDBByodbc()
{
	int		iTagDb = 0; //0 成功   1 异常

	CString		strCon;


	//CDatabase  mydb[20];

	//mydb->OpenEx()

	strCon.Format("DSN=%s;UID=%s;PWD=%s", g_dbSource, g_dbUse, g_dbpw);
	TRY
	{

		//打开数据库
		if (!myDB.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("通过数据源连接数据库失败");
			return -1;
		}
		//系统启动的时候 建立了一条连接 (常连接)
	}
	CATCH_ALL(e)
	{
		iTagDb = 1;
		e->ReportError();
	}
	END_CATCH_ALL

	if (iTagDb == 1)
	{
		AfxMessageBox("通过数据源连接数据库 err");
		return -1;
	}
	else
	{
		//AfxMessageBox("通过数据源连接数据库 ok");
	}
	g_pDB = &myDB;
	return 0;
}

BOOL CMyAdminApp::InitInstance()
{
	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	//读配置文件
	int			ret = 0;
	ret = ReadConfig();
	if (ret != 0)
	{
		CDlgInitCfg  dlgInitCfg;
		if (dlgInitCfg.DoModal() == IDCANCEL)
		{
			AfxMessageBox("读配置文件失败");
			return FALSE;
		}
		else
		{
			 g_dbSource = dlgInitCfg.m_strDbSource;
			 g_dbUse = dlgInitCfg.m_strDbUser;
			 g_dbpw = dlgInitCfg.m_strDbPw;
		}
	}
	//连接数据库
	ret = ConnectDBByodbc();
	if (ret != 0)
	{
		AfxMessageBox("通过数据源连接数据库 err");
		return FALSE;
	}
	//AfxMessageBox("通过数据源连接数据库 ok");
	



	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CMyAdminView));
	if (!pDocTemplate)
		return FALSE;
	this->AddDocTemplate(pDocTemplate);
	


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CMyAdminApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CMyAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMyAdminApp 消息处理程序



