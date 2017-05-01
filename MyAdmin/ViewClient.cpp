// ViewClient.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "ViewClient.h"
#include "keymngclientop.h"


// CViewClient

IMPLEMENT_DYNCREATE(CViewClient, CFormView)

CViewClient::CViewClient()
	: CFormView(CViewClient::IDD)
{

}

CViewClient::~CViewClient()
{
}

void CViewClient::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENTNODE, m_listSecNode);
}

BEGIN_MESSAGE_MAP(CViewClient, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_KEYAGREE, &CViewClient::OnBnClickedButtonKeyagree)
	ON_BN_CLICKED(IDC_BUTTON_CLIENTINIT, &CViewClient::OnBnClickedButtonClientinit)
	ON_BN_CLICKED(IDC_BUTTON_KEYCHECK, &CViewClient::OnBnClickedButtonKeycheck)
END_MESSAGE_MAP()


// CViewClient 诊断

#ifdef _DEBUG
void CViewClient::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewClient::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewClient 消息处理程序


void CViewClient::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基
	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);


	//通过编程的方式 或者 是属性界面方式去修改 CWnd类资源属性
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);
	m_listSecNode.InsertColumn(0, "client编号", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "server编号", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "当前密钥keyid", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "密钥状态", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "请求时间", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);


	
	CString myclientid = "1111";
	CString myserverid = "0001";
	int state = 0;
	int keyid = 100;
	CTime time = CTime::GetCurrentTime();
	DbInitListSecNode(myclientid, myserverid, time, state, keyid);
	

}


//int CViewClient::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
int CViewClient::DbInitListSecNode(CString &clientid, CString &serverid, CTime &time, int state, int keyid)
{
	LVITEM		lvi;

	//设置0列
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iImage = 4;
	lvi.iItem = 0; //头插 //在第几行插入数据
	lvi.iSubItem = 0; //第几列
	lvi.pszText = (LPTSTR)(LPCTSTR)clientid;
	m_listSecNode.InsertItem(&lvi);

	//设置1列
	lvi.iSubItem = 1; //第几列
	lvi.pszText = (LPTSTR)(LPCTSTR)serverid;
	m_listSecNode.SetItem(&lvi);


	//设置2列
	CString str = "";
	str.Format("%d", keyid);

	lvi.iSubItem = 2;
	lvi.pszText = (LPTSTR)(LPCTSTR)str;
	m_listSecNode.SetItem(&lvi);

	//设置3列
	lvi.iSubItem = 3; //第几列
	if (state == 0)
	{
		lvi.pszText = "正常";
	}
	else
	{
		lvi.pszText = "不正常";
	}
	m_listSecNode.SetItem(&lvi);


	//设置4列
	lvi.iSubItem = 4; //第几列
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	return 0;
}


MngClient_Info				mngClientInfo;

void CViewClient::OnBnClickedButtonKeyagree()
{
	// TODO:  在此添加控件通知处理程序代码
	int ret = 0;
	ret = MngClient_Agree(&mngClientInfo);
	if (ret != 0)
	{
		AfxMessageBox("func MngClient_Agree() err:%d \n", ret);
		return;
	}
	else
	{
		AfxMessageBox("func MngClient_Agree() ok:%d \n", ret);
	}

}


void CViewClient::OnBnClickedButtonClientinit()
{

	int ret = 0;

	memset(&mngClientInfo, 0, sizeof(mngClientInfo));

	//系统初始化
	ret = MngClient_InitInfo(&mngClientInfo);
	if (ret != 0)
	{
		AfxMessageBox("func MngClient_InitInfo() err:%d \n", ret);
		return;
	}
	else
	{
		AfxMessageBox("func MngClient_InitInfo() ok:%d \n", ret);
	}
}


void CViewClient::OnBnClickedButtonKeycheck()
{
	// TODO:  在此添加控件通知处理程序代码
	//系统初始化
	int ret = 0;
	ret = MngClient_Check(&mngClientInfo);
	if (ret != 0)
	{
		AfxMessageBox("func MngClient_InitInfo() err:%d \n", ret);
		return;
	}
	else
	{
		AfxMessageBox("func MngClient_InitInfo() ok:%d \n", ret);
	}
}
