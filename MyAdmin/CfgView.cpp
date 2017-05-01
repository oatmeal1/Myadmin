// CfgView.cpp : 实现文件
//

#include "stdafx.h"
#include "MyAdmin.h"
#include "CfgView.h"
#include "SECMNGSRVCFG.h"


// CCfgView

IMPLEMENT_DYNCREATE(CCfgView, CFormView)

CCfgView::CCfgView()
	: CFormView(CCfgView::IDD)
	, m_strSrvIp(_T(""))
	, m_strSrvPort(_T(""))
	, m_strSrvMaxNodeNum(_T(""))
{

}

CCfgView::~CCfgView()
{
}

void CCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRVIP, m_strSrvIp);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_strSrvPort);
	DDX_Text(pDX, IDC_EDIT_SRVMAXNODE, m_strSrvMaxNodeNum);
}

BEGIN_MESSAGE_MAP(CCfgView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SRVSAVE, &CCfgView::OnBnClickedButtonSrvsave)
END_MESSAGE_MAP()


// CCfgView 诊断

#ifdef _DEBUG
void CCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCfgView 消息处理程序


extern CDatabase	*g_pDB ;

void CCfgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	//从数据库中获取 信息  初始化界面

	CSECMNGSRVCFG  srvCfgSet(g_pDB);

	TRY 
	{
		//	select * from secmng.srvcfg where key = 'secmng_server_ip';
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none);  //

		if (!srvCfgSet.IsEOF())
		{
			//把数据库记录中的valude 给到 C++界面变量
			m_strSrvIp = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strSrvIp = "";
		}

		//重复查询
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();  //win7 oracle11
		if (!srvCfgSet.IsEOF())
		{
			//把数据库记录中的valude 给到 C++界面变量
			m_strSrvPort =  srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strSrvPort = "";
		}

		//重复查询
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())
		{
			//把数据库记录中的valude 给到 C++界面变量
			m_strSrvMaxNodeNum = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strSrvMaxNodeNum = "";
		}
	}
	CATCH_ALL ( e)
	{
		e->ReportError();
		if (srvCfgSet.IsOpen())
		{
			srvCfgSet.Close();
		}
	}
	END_CATCH_ALL
	
	
	UpdateData(FALSE);




	// TODO:  在此添加专用代码和/或调用基类
}


//1 若表中记录已经存在，则修改；
//2 若表中记录不存在，则新增

void CCfgView::OnBnClickedButtonSrvsave()
{
	int				dbTag = 0; //0 正常    1异常
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE); //用户的输入 DDX 类的成员变量
	m_strSrvIp.TrimLeft();
	m_strSrvIp.TrimRight();

	m_strSrvMaxNodeNum.TrimLeft();
	m_strSrvMaxNodeNum.TrimRight();
	m_strSrvPort.TrimLeft();
	m_strSrvPort.TrimRight();

	if (m_strSrvIp.IsEmpty() ||
		m_strSrvMaxNodeNum.IsEmpty() ||
		m_strSrvPort.IsEmpty())
	{
		AfxMessageBox("用户的输入不能为空");
		return;
	}

	CSECMNGSRVCFG  srvCfgSet(g_pDB);
	g_pDB->BeginTrans();

	TRY
	{
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none);  //
		if (!srvCfgSet.IsEOF())  //若记录存在  则修改
		{
			//把数据库记录中的valude 给到 C++界面变量
			srvCfgSet.Edit();
			srvCfgSet.m_KEY= "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strSrvIp;
			srvCfgSet.Update();

		} 
		else  //若没有记录  新增
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strSrvIp;
			srvCfgSet.Update();
		}

		//保存port
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())  //若记录存在  则修改
		{
			//把数据库记录中的valude 给到 C++界面变量
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strSrvPort;
			srvCfgSet.Update();

		}
		else  //若没有记录  新增
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strSrvPort;
			srvCfgSet.Update();
		}

		//保存maxndoe
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())  //若记录存在  则修改
		{
			//把数据库记录中的valude 给到 C++界面变量
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_maxnetnum";
			srvCfgSet.m_VALUDE = m_strSrvMaxNodeNum;
			srvCfgSet.Update();

		}
		else  //若没有记录  新增
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_maxnetnum";
			srvCfgSet.m_VALUDE = m_strSrvMaxNodeNum;
			srvCfgSet.Update();
		}

	}
	CATCH_ALL(e)
	{
		dbTag = 1;
		e->ReportError();
		
	}
	END_CATCH_ALL


	if (dbTag == 0)
	{
		g_pDB->CommitTrans();
	}
	else
	{
		g_pDB->Rollback();
	}

	if (srvCfgSet.IsOpen())
	{
		srvCfgSet.Close();
	}

	AfxMessageBox("保存成功");
}
