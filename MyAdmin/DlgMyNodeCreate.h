#pragma once


// CDlgMyNodeCreate �Ի���

class CDlgMyNodeCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgMyNodeCreate)

public:
	CDlgMyNodeCreate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMyNodeCreate();

// �Ի�������
	enum { IDD = IDD_DIALOG_MYNODECREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNodeId;
};
