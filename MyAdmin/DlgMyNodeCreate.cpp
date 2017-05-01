// DlgMyNodeCreate.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgMyNodeCreate.h"
#include "afxdialogex.h"


// CDlgMyNodeCreate 对话框

IMPLEMENT_DYNAMIC(CDlgMyNodeCreate, CDialog)

CDlgMyNodeCreate::CDlgMyNodeCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMyNodeCreate::IDD, pParent)
	, m_strNodeId(_T(""))
{

}

CDlgMyNodeCreate::~CDlgMyNodeCreate()
{
}

void CDlgMyNodeCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NODEID, m_strNodeId);
}


BEGIN_MESSAGE_MAP(CDlgMyNodeCreate, CDialog)
END_MESSAGE_MAP()


// CDlgMyNodeCreate 消息处理程序
