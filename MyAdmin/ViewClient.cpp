// ViewClient.cpp : ʵ���ļ�
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


// CViewClient ���

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


// CViewClient ��Ϣ�������


void CViewClient::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û�
	HIMAGELIST hList = ImageList_Create(16, 16, ILC_COLOR8 | ILC_MASK, 8, 1);
	m_imageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_BITMAP_SECNODE);
	m_imageList.Add(&cBmp, RGB(255, 0, 255));
	cBmp.DeleteObject();

	m_listSecNode.SetImageList(&m_imageList, LVSIL_SMALL);


	//ͨ����̵ķ�ʽ ���� �����Խ��淽ʽȥ�޸� CWnd����Դ����
	DWORD dwExStyle = ListView_GetExtendedListViewStyle(m_listSecNode.m_hWnd);
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_listSecNode.m_hWnd, dwExStyle);

	CRect rect; //msdn
	m_listSecNode.GetClientRect(&rect);
	int nColInterval = rect.Width() / 5;

	m_listSecNode.SetRedraw(FALSE);
	m_listSecNode.InsertColumn(0, "client���", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(1, "server���", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(2, "��ǰ��Կkeyid", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(3, "��Կ״̬", LVCFMT_LEFT, nColInterval);
	m_listSecNode.InsertColumn(4, "����ʱ��", LVCFMT_LEFT, rect.Width() - 4 * nColInterval);
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

	//����0��
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
	lvi.iImage = 4;
	lvi.iItem = 0; //ͷ�� //�ڵڼ��в�������
	lvi.iSubItem = 0; //�ڼ���
	lvi.pszText = (LPTSTR)(LPCTSTR)clientid;
	m_listSecNode.InsertItem(&lvi);

	//����1��
	lvi.iSubItem = 1; //�ڼ���
	lvi.pszText = (LPTSTR)(LPCTSTR)serverid;
	m_listSecNode.SetItem(&lvi);


	//����2��
	CString str = "";
	str.Format("%d", keyid);

	lvi.iSubItem = 2;
	lvi.pszText = (LPTSTR)(LPCTSTR)str;
	m_listSecNode.SetItem(&lvi);

	//����3��
	lvi.iSubItem = 3; //�ڼ���
	if (state == 0)
	{
		lvi.pszText = "����";
	}
	else
	{
		lvi.pszText = "������";
	}
	m_listSecNode.SetItem(&lvi);


	//����4��
	lvi.iSubItem = 4; //�ڼ���
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S");
	lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
	m_listSecNode.SetItem(&lvi);

	return 0;
}


MngClient_Info				mngClientInfo;

void CViewClient::OnBnClickedButtonKeyagree()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	//ϵͳ��ʼ��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//ϵͳ��ʼ��
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
