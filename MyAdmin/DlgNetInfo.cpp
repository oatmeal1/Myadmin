// DlgNetInfo.cpp : ʵ���ļ�
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


// CDlgNetInfo ���

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


// CDlgNetInfo ��Ϣ�������


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
	m_listSecNode.InsertColumn(0, "������", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "��������", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "���㴴��ʱ��", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "����״̬", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "������Ȩ��", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
	m_listSecNode.SetRedraw(TRUE);



}

/* 
typedef struct tagLVITEMW
{
	UINT mask;
	int iItem;  //�ڵڼ��в������� 
	int iSubItem;  //�ڼ�����
	UINT state;
	UINT stateMask;
	LPWSTR pszText;  //�������
	int cchTextMax;
	int iImage;  //ͼƬ�б�������±�
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


//����:
//1�������õ�ʱ�� : ��C++�����������ǳ���Ա ȡʵ�εĵ�ַ �����β�
//2 ����������βα��������˵Ⱥŵ��ұ� = id   ���ݻ�Ϊ  =*id
//2 ����������βα��������˵Ⱥŵ��� id = ���ݻ�Ϊ  *id = 
int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode)
{
	CString strTime;
	LVITEM   lvi;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0; //�ڵ�0�в���һ��
	lvi.iSubItem = 0;
	lvi.iImage = 4;
	lvi.pszText = (LPTSTR)(LPCTSTR)ID;
	m_listSecNode.InsertItem(&lvi);

	//���õ�0�еĵ�2��
	lvi.iSubItem = 1;
	lvi.pszText = (LPTSTR)(LPCTSTR)Name;
	m_listSecNode.SetItem(&lvi);

	//���õ�0�еĵ�3��
	strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.iSubItem = 2;
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	//���õ�0�еĵ�4��
	char		buf[24] = { 0 };
	sprintf(buf, "%d", state);
	
	lvi.iSubItem = 3;
	lvi.pszText = buf;
	m_listSecNode.SetItem(&lvi);


	//���õ�0�еĵ�5��
	
	sprintf(buf, "%d", authcode);

	lvi.iSubItem = 4;
	lvi.pszText = buf;
	m_listSecNode.SetItem(&lvi);

	return 0;
}


void CDlgNetInfo::OnBnClickedButtonNodecreate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString  myID = "0001";
	CString myName = "������";
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

	//�ô� ���� ͨ������ ���ɴ�������Ķ��� 
	//

	//CWnd *myWnd = CWnd::GetDlgItem(IDC_CHECK_TIME);
	CWnd *myWnd = GetDlgItem(IDC_CHECK_TIME);
	CButton  *myButton = NULL;

	myButton = (CButton *)myWnd;

	if (myButton->GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox("ʱ��ؼ�û�б�ѡ�� ");
		return;
	}

	CTime sqlTime1(m_dateBegin.GetYear(), m_dateBegin.GetMonth(), m_dateBegin.GetDay(),
		0, 0, 0);
	CTime sqlTime2(m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(),
		23, 59, 59);

	if (sqlTime1 >= sqlTime2)
	{
		MessageBox("��ʼʱ�䲻�ܴ��ڽ���ʱ�䣡", "ʱ���ѯ", MB_MODEMASK);
		return;
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSECMNGSECNODE  secMngSecNode(g_pDB);
	TRY 
	{
		if (!secMngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			MessageBox("��CSECMNGSECNODE��ʧ�ܣ�", "���ݿ����", MB_MODEMASK);
			return;
		}
		//��open�ɹ�  �����Ѿ��������ǵĿͻ���
		m_listSecNode.SetRedraw(FALSE); ////��ֹˢ�¿ؼ�,�����ظ�������Ŀʱ,����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CDlgNetInfo::OnBnClickedButtonNodedel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int			dbTag = 0;
	UpdateData(TRUE);
	POSITION pos = m_listSecNode.GetFirstSelectedItemPosition();
	int nItem = m_listSecNode.GetNextSelectedItem(pos); //�õ�������
	CString myId = m_listSecNode.GetItemText(nItem, 0);
	CString myName = m_listSecNode.GetItemText(nItem, 1);
	//AfxMessageBox(myId + myName);

	CString strTmp;
	strTmp.Format("�Ƿ�Ҫɾ�����Ϊ��%s����������Ϣ��", myId);
	if (AfxMessageBox(strTmp, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
	{
		return;
	}


	//ɾ�����ݿ��еļ�¼

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSECMNGSECNODE  secMngSecNode(g_pDB);
	g_pDB->BeginTrans();
	TRY
	{
		secMngSecNode.m_strFilter.Format("id = '%s' ", myId);
		if (!secMngSecNode.Open(CRecordset::snapshot, NULL, CRecordset::none))
		{
			MessageBox("��CSECMNGSECNODE��ʧ�ܣ�", "���ݿ����", MB_MODEMASK);
			return;
		}

		if (!secMngSecNode.IsEOF())
		{
			
			secMngSecNode.Delete();
		}
		else
		{
			MessageBox("û�м��������������ļ�¼��", "���ݿ����", MB_MODEMASK);
			
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

	//ɾ������
	m_listSecNode.DeleteItem(nItem);

}
