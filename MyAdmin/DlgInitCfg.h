#pragma once


// CDlgInitCfg �Ի���

class CDlgInitCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgInitCfg)

public:
	CDlgInitCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInitCfg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DBINIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strDbPw;
	CString m_strDbSource;
	CString m_strDbUser;
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButtonDbtest();
};
