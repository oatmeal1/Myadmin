// DlgInitCfg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgInitCfg.h"
#include "afxdialogex.h"
#include "cfg_op.h"


// CDlgInitCfg �Ի���

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


// CDlgInitCfg ��Ϣ�������


void CDlgInitCfg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//���û��� �����ϵ�����  =====> C++����
	UpdateData(TRUE);
	

	//�����ݱ��浽�����ļ���

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

	//д������
	//dbsource
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "DSN", (LPTSTR)(LPCTSTR)m_strDbSource/*in*/, m_strDbSource.GetLength() );
	if (ret != 0)
	{
		AfxMessageBox("д������ DSN ʧ��");
		return;
	}

	//UID 
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "UID", (LPTSTR)(LPCTSTR)m_strDbUser/*in*/, m_strDbUser.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("д������ UID ʧ��");
		return;
	}

	//PWD
	ret = WriteCfgItem((LPTSTR)(LPCTSTR)strFilePath, "PWD", (LPTSTR)(LPCTSTR)m_strDbPw/*in*/, m_strDbPw.GetLength());
	if (ret != 0)
	{
		AfxMessageBox("д������ PWD ʧ��");
		return;
	}
	CDialog::OnOK();
}




void CDlgInitCfg::OnBnClickedButtonDbtest()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString		strCon;
	UpdateData(TRUE);
	strCon.Format("DSN=%s;UID=%s;PWD=%s", m_strDbSource, m_strDbUser, m_strDbPw);

	CDatabase  db;

	TRY
	{
		if (!db.OpenEx(strCon, CDatabase::noOdbcDialog))
		{
			AfxMessageBox("���� �������ݿ�ʧ�� ,���������� ");
			return;
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		AfxMessageBox("���� �������ݿ�ʧ�� ,���������� ");
		return;
	}
	END_CATCH_ALL
	AfxMessageBox("���� �������ݿ�0k");
	if (db.IsOpen())
	{
		db.Close();
	}
	
}
