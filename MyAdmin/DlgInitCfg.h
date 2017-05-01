#pragma once


// CDlgInitCfg 对话框

class CDlgInitCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgInitCfg)

public:
	CDlgInitCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInitCfg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DBINIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strDbPw;
	CString m_strDbSource;
	CString m_strDbUser;
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButtonDbtest();
};
