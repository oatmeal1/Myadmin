#pragma once
#include "afxcmn.h"



// CViewClient ������ͼ

class CViewClient : public CFormView
{
	DECLARE_DYNCREATE(CViewClient)

protected:
	CViewClient();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewClient();

public:
	enum { IDD = IDD_DIALOG_CLIENT };
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
	afx_msg void OnBnClickedButtonKeyagree();
	int CViewClient::DbInitListSecNode(CString &clientid, CString &serverid, CTime &time, int state, int keyid);

	afx_msg void OnBnClickedButtonClientinit();
	afx_msg void OnBnClickedButtonKeycheck();
};


