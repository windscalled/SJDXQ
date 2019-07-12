// Modify.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Modify.h"
#include "afxdialogex.h"


// CModify �Ի���

IMPLEMENT_DYNAMIC(CModify, CDialogEx)

CModify::CModify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModify::IDD, pParent)
	, findid(_T(""))
{
	
}

CModify::~CModify()
{
}

void CModify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, findid);
}


BEGIN_MESSAGE_MAP(CModify, CDialogEx)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CModify::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CModify ��Ϣ�������


void CModify::OnBnClickedButton1()   //ȷ����ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(findid == "")
	{
		return;
	}		
	else if(findid.GetLength()!=6)
	{
		MessageBox(_T("�Բ��������˺���������"));
		return;
	}

	if(theApp.search != 1)
	{
		CString st("QQ�ͻ��˲��ҷ�����");
		theApp.myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//���͹㲥
		MessageBox(_T("δ��������������"));
		return;
	}

	theApp.sereverip = theApp.myudp->s;//������IP
	theApp.mytcp = new CTcpSocket();
	if(!theApp.mytcp)
	{
		MessageBox(_T("�ڴ治�㣡"));
		return;
	}
	else if(!theApp.mytcp->Create())
	{

		MessageBox(_T("�����׽���ʧ�ܣ�"));

		return;
	}
	else if(!theApp.mytcp->Connect(theApp.sereverip,1003)) 
	{		
		MessageBox(_T("���ӷ�����ʧ��?"));
		return;
	}
	//theApp.mytcp->Init();

	CHead temp;
	CString only(theApp.mylocalip);
	only += "00001";
	temp.type = 6;		//���˺ŷ��͸�������
	theApp.m_userid = findid ;
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy_s(temp.message,(CStringA)only);
	strcpy(temp.id,(CStringA)findid);
	theApp.mytcp->Send(&temp,sizeof(temp));

	
}



LRESULT CModify::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.modify_tag == 1)
	{	
		theApp.modify_tag = 0 ;//���ø���ֵ
		theApp.ques = new CQuestion() ;
		theApp.ques->Create(IDD_DIALOG3,NULL);
		theApp.ques->ShowWindow(SW_SHOW) ;
		theApp.mod->ShowWindow(SW_HIDE) ;				
	}
	else
	{
		MessageBox(_T("�Բ��𣬸��˺���δע�ᣬ���������룡"),_T("QQ"));
	}

	return TRUE;
}


BOOL CModify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strPath = TEXT("../../picture/4.bmp");
	if(m_image1.Load((LPCTSTR)strPath) != S_OK) //����ͼƬ·������ͼƬ 
	{
		AfxMessageBox(TEXT("Load Error!"));
		return FALSE;
	}
	HBITMAP hBit = m_image1;
	CBitmap cBit;
	m_Br2.DeleteObject() ; //������ʼ��
	CBitmap* pBit = cBit.FromHandle(hBit);
	m_Br2.CreatePatternBrush(pBit);
	cBit.DeleteObject();

	HBITMAP hbit_button1, hbit_button2;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/confirm.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"����");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CModify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CModify::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow() ;
}


void CModify::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::PostNcDestroy();

	delete theApp.mod ;
	theApp.mod = NULL ;
}
