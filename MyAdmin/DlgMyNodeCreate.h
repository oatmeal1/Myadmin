#pragma once


// CDlgMyNodeCreate 对话框

class CDlgMyNodeCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgMyNodeCreate)

public:
	CDlgMyNodeCreate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMyNodeCreate();

// 对话框数据
	enum { IDD = IDD_DIALOG_MYNODECREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNodeId;
};
