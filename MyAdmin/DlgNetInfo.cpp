// DlgNetInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "DlgNetInfo.h"
#include "SECMNGSECNODE.h"
#include "DlgMyNodeCreate.h"


// CDlgNetInfo

IMPLEMENT_DYNCREATE(CDlgNetInfo, CFormView)

CDlgNetInfo::CDlgNetInfo()
	: CFormView(CDlgNetInfo::IDD)
	, m_dateBegin(COleDateTime::GetCurrentTime())
	, m_dateEnd(COleDateTime::GetCurrentTime())
{

}

CDlgNetInfo::~CDlgNetInfo()
{
}

void CDlgNetInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECNODE, m_listSecNode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dateEnd);
}

BEGIN_MESSAGE_MAP(CDlgNetInfo, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_NODECREATE, &CDlgNetInfo::OnBnClickedButtonNodecreate)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDlgNetInfo::OnBnClickedButtonSearch)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgNetInfo::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON_NODEDEL, &CDlgNetInfo::OnBnClickedButtonNodedel)
END_MESSAGE_MAP()


// CDlgNetInfo 诊断

#ifdef _DEBUG
void CDlgNetInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgNetInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgNetInfo 消息处理程序


void CDlgNetInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);

	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);  //CListCtrl 
	m_listSecNode.InsertColumn(0, "网点编号", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "网点名称", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "网点创建时间", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "网点状态", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "网点授权码", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);



}

/* 
typedef struct tagLVITEMW
{
	UINT mask;
	int iItem;  //在第几行插入数据 
	int iSubItem;  //第几个列
	UINT state;
	UINT stateMask;
	LPWSTR pszText;  //结点文字
	int cchTextMax;
	int iImage;  //图片列表的数组下标
	LPARAM lParam;
	int iIndent;
#if (NTDDI_VERSION >= NTDDI_WINXP)
	int iGroupId;
	UINT cColumns; // tile view columns
	PUINT puColumns;
#endif
#if (NTDDI_VERSION >= NTDDI_VISTA)
	int* piColFmt;
	int iGroup; // readonly. only valid for owner data.
#endif
} LVITEMW, *LPLVITEMW;
*/


//结论:
//1函数调用的时候 : 是C++编译器把我们程序员 取实参的地址 传给形参
//2 若引用这个形参变量放在了等号的右边 = id   会演化为  =*id
//2 若引用这个形参变量放在了等号的做 id = 会演化为  *id = 
int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
{
	CString strTime;
	LVITEM   lvi;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0; //在第0行插入一行
	lvi.iSubItem = 0;
	lvi.iImage = 4;
	lvi.pszText = (LPTSTR)(LPCTSTR)ID;
	m_listSecNode.InsertItem(&lvi);

	//设置第0行的第2列
	lvi.iSubItem = 1;
	lvi.pszText = (LPTSTR)(LPCTSTR)Name;
	m_listSecNode.SetItem(&lvi);

	//设置第0行的第3列
	strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.iSubItem = 2;
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	//设置第0行的第4列
	char		buf[24] = { 0 };
	sprintf(buf, "%d", state);
	
	lvi.iSubItem = 3;
	lvi.pszText = buf;
	m_listSecNode.SetItem(&lvi);


	//设置第0行的第5列
	
	sprintf(buf, "%d", authcode);

	lvi.iSubItem = 4;
	lvi.pszText = buf;
	m_listSecNode.SetItem(&lvi);

	return 0;
}


void CDlgNetInfo::OnBnClickedButtonNodecreate()
{
	// TODO:  在此添加控件通知处理程序代码
	CString  myID = "0001";
	CString myName = "根网点";
	CTime mytime = CTime::GetCurrentTime();
	int state = 0;
	int authcode = 0x1;
	

	CDlgMyNodeCreate   dlgMyNodeCreate;
	if (dlgMyNodeCreate.DoModal() == IDCANCEL)
	{
		return;
	}

	//
	dlgMyNodeCreate.m_strNodeId;



	//DbInitListSecNode(myID, myName, mytime, state, authcode);
}

extern CDatabase *g_pDB;

/* 
CWnd* GetDlgItem(int nID)
{
	return new  CWnd();
}
*/

void CDlgNetInfo::OnBnClickedButtonSearch()
{
	UpdateData(TRUE);

	//好处 避免 通过类向导 生成大量的类的对象 
	//

	//CWnd *myWnd = CWnd::GetDlgItem(IDC_CHECK_TIME);
	CWnd *myWnd = GetDlgItem(IDC_CHECK_TIME);
	CButton  *myButton = NULL;

	myButton = (CButton *)myWnd;

	if (myButton->GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox("时间控件没有被选中 ");
		return;
	}

	CTime sqlTime1(m_dateBegin.GetYear(), m_dateBegin.GetMonth(), m_dateBegin.GetDay(),
		0, 0, 0);
	CTime sqlTime2(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(),
		23, 59, 59);

	if (sqlTime1 >= sqlTime2)
	{
		MessageBox("开始时间不能大于结束时间！", "时间查询", MB_MODEMASK);
		return;
	}

	// TODO:  在此添加控件通知处理程序代码
	CSECMNGSECNODE  secMngSecNode(g_pDB);
	TRY 
	{
		if (!secMngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			MessageBox("打开CSECMNGSECNODE表失败！", "数据库操作", MB_MODEMASK);
			return;
		}
		//若open成功  数据已经来到我们的客户端
		m_listSecNode.SetRedraw(FALSE); ////禁止刷新控件,避免重复插入条目时,闪屏
		m_listSecNode.DeleteAllItems();
		

		while (!secMngSecNode.IsEOF())
		{
			if (!(secMngSecNode.m_CREATETIME >= sqlTime1 &&
				secMngSecNode.m_CREATETIME <= sqlTime2))
			{
				secMngSecNode.MoveNext();
				continue;
			}

			DbInitListSecNode(secMngSecNode.m_ID, secMngSecNode.m_NAME, 
				secMngSecNode.m_CREATETIME, secMngSecNode.m_STATE, secMngSecNode.m_AUTHCODE);
			secMngSecNode.MoveNext();
		}

		m_listSecNode.SetRedraw(TRUE);
	}
	CATCH_ALL ( e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
	
	if (secMngSecNode.IsOpen())
	{
		secMngSecNode.Close();
	}

}


void CDlgNetInfo::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgNetInfo::OnBnClickedButtonNodedel()
{
	// TODO:  在此添加控件通知处理程序代码
	int			dbTag = 0;
	UpdateData(TRUE);
	POSITION pos = m_listSecNode.GetFirstSelectedItemPosition();
	int nItem = m_listSecNode.GetNextSelectedItem(pos); //得到几几行
	CString myId = m_listSecNode.GetItemText(nItem, 0);
	CString myName = m_listSecNode.GetItemText(nItem, 1);
	//AfxMessageBox(myId + myName);

	CString strTmp;
	strTmp.Format("是否要删除编号为【%s】的网点信息吗？", myId);
	if (AfxMessageBox(strTmp, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
	{
		return;
	}


	//删除数据库中的记录

	// TODO:  在此添加控件通知处理程序代码
	CSECMNGSECNODE  secMngSecNode(g_pDB);
	g_pDB->BeginTrans();
	TRY
	{
		secMngSecNode.m_strFilter.Format("id = '%s' ", myId);
		if (!secMngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			MessageBox("打开CSECMNGSECNODE表失败！", "数据库操作", MB_MODEMASK);
			return;
		}

		if (!secMngSecNode.IsEOF())
		{
			
			secMngSecNode.Delete();
		}
		else
		{
			MessageBox("没有检索到符合条件的记录！", "数据库操作", MB_MODEMASK);
			
			if (secMngSecNode.IsOpen())
			{
				secMngSecNode.Close();
			}
			return;
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		dbTag = 1;
	}
	END_CATCH_ALL

	if (dbTag == 1)
	{
		g_pDB->Rollback();
	}
	else
	{
		g_pDB->CommitTrans();
	}

	if (secMngSecNode.IsOpen())
	{
		secMngSecNode.Close();
	}

	//删除界面
	m_listSecNode.DeleteItem(nItem);

}
