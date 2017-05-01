// CfgView.cpp : ʵ���ļ�
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


// CCfgView ���

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


// CCfgView ��Ϣ�������


extern CDatabase	*g_pDB ;

void CCfgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	//�����ݿ��л�ȡ ��Ϣ  ��ʼ������

	CSECMNGSRVCFG  srvCfgSet(g_pDB);

	TRY 
	{
		//	select * from secmng.srvcfg where key = 'secmng_server_ip';
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none);  //

		if (!srvCfgSet.IsEOF())
		{
			//�����ݿ��¼�е�valude ���� C++�������
			m_strSrvIp = srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strSrvIp = "";
		}

		//�ظ���ѯ
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();  //win7 oracle11
		if (!srvCfgSet.IsEOF())
		{
			//�����ݿ��¼�е�valude ���� C++�������
			m_strSrvPort =  srvCfgSet.m_VALUDE;
		}
		else
		{
			m_strSrvPort = "";
		}

		//�ظ���ѯ
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())
		{
			//�����ݿ��¼�е�valude ���� C++�������
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




	// TODO:  �ڴ����ר�ô����/����û���
}


//1 �����м�¼�Ѿ����ڣ����޸ģ�
//2 �����м�¼�����ڣ�������

void CCfgView::OnBnClickedButtonSrvsave()
{
	int				dbTag = 0; //0 ����    1�쳣
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE); //�û������� DDX ��ĳ�Ա����
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
		AfxMessageBox("�û������벻��Ϊ��");
		return;
	}

	CSECMNGSRVCFG  srvCfgSet(g_pDB);
	g_pDB->BeginTrans();

	TRY
	{
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_ip");
		srvCfgSet.Open(CRecordset::snapshot, NULL, CRecordset::none);  //
		if (!srvCfgSet.IsEOF())  //����¼����  ���޸�
		{
			//�����ݿ��¼�е�valude ���� C++�������
			srvCfgSet.Edit();
			srvCfgSet.m_KEY= "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strSrvIp;
			srvCfgSet.Update();

		} 
		else  //��û�м�¼  ����
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_ip";
			srvCfgSet.m_VALUDE = m_strSrvIp;
			srvCfgSet.Update();
		}

		//����port
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_port");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())  //����¼����  ���޸�
		{
			//�����ݿ��¼�е�valude ���� C++�������
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strSrvPort;
			srvCfgSet.Update();

		}
		else  //��û�м�¼  ����
		{
			srvCfgSet.AddNew();
			srvCfgSet.m_KEY = "secmng_server_port";
			srvCfgSet.m_VALUDE = m_strSrvPort;
			srvCfgSet.Update();
		}

		//����maxndoe
		srvCfgSet.m_strFilter.Format("key  = '%s'", "secmng_server_maxnetnum");
		srvCfgSet.Requery();
		if (!srvCfgSet.IsEOF())  //����¼����  ���޸�
		{
			//�����ݿ��¼�е�valude ���� C++�������
			srvCfgSet.Edit();
			srvCfgSet.m_KEY = "secmng_server_maxnetnum";
			srvCfgSet.m_VALUDE = m_strSrvMaxNodeNum;
			srvCfgSet.Update();

		}
		else  //��û�м�¼  ����
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

	AfxMessageBox("����ɹ�");
}
