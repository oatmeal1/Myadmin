// DlgInitCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgInitCfg.h"
#include "afxdialogex.h"
#include "cfg_op.h"


// CDlgInitCfg 对话框

IMPLEMENT_DYNAMIC(CDlgInitCfg, CDialog)

CDlgInitCfg::CDlgInitCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInitCfg::IDD, pParent)
	, m_strDbPw(_T(""))
	, m_strDbSource(_T(""))
	, m_strDbUser(_T(""))
{

}

CDlgInitCfg::~CDlgInitCfg()
{
}

void CDlgInitCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBPW, m_strDbPw);
	DDX_Text(pDX, IDC_EDIT_DBSOURCE, m_strDbSource);
	DDX_Text(pDX, IDC_EDIT_DBUSER, m_strDbUser);
}


BEGIN_MESSAGE_MAP(CDlgInitCfg, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInitCfg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DBTEST, &CDlgInitCfg::OnBnClickedButtonDbtest)
END_MESSAGE_MAP()


// CDlgInitCfg 消息处理程序


void CDlgInitCfg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	//把用户的 界面上的输入  =====> C++变量
	UpdateData(TRUE);
	

	//把数据保存到配置文件中

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

	//写配置项
	//dbsource
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "DSN", (LPTSTR)(LPCTSTR)m_strDbSource/*in*/, m_strDbSource.GetLength() );
	if (ret != 0)
	{
		AfxMessageBox("写配置项 DSN 失败");
		return;
	}

	//UID 
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "UID", (LPTSTR)(LPCTSTR)m_strDbUser/*in*/, m_strDbUser.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("写配置项 UID 失败");
		return;
	}

	//PWD
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "PWD", (LPTSTR)(LPCTSTR)m_strDbPw/*in*/, m_strDbPw.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("写配置项 PWD 失败");
		return;
	}
	CDialog::OnOK();
}




void CDlgInitCfg::OnBnClickedButtonDbtest()
{
	// TODO:  在此添加控件通知处理程序代码
	CString		strCon;
	UpdateData(TRUE);
	strCon.Format("DSN=%s;UID=%s;PWD=%s", m_strDbSource, m_strDbUser, m_strDbPw);

	CDatabase  db;

	TRY
	{
		if (!db.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("测试 连接数据库失败 ,请重新配置 ");
			return;
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		AfxMessageBox("测试 连接数据库失败 ,请重新配置 ");
		return;
	}
	END_CATCH_ALL
	AfxMessageBox("测试 连接数据库0k");
	if (db.IsOpen())
	{
		db.Close();
	}
	
}
