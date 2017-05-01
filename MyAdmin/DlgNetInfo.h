#pragma once
#include "afxcmn.h"
#include "ATLComTime.h"



// CDlgNetInfo ������ͼ

class CDlgNetInfo : public CFormView
{
	DECLARE_DYNCREATE(CDlgNetInfo)

protected:
	CDlgNetInfo();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgNetInfo();

public:
	enum { IDD = IDD_DIALOG_NETMNG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listSecNode;
	virtual void OnInitialUpdate();
	
public:
	CImageList m_imageList;
	afx_msg void OnBnClickedButtonNodecreate();

public:
	int CDlgNetInfo::DbInitListSecNode(CString &ID, CString &Name, CTime &time, int state, int authcode);

	afx_msg void OnBnClickedButtonSearch();
	COleDateTime m_dateBegin;
	COleDateTime m_dateEnd;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNodedel();
};


