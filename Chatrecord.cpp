// Chatrecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Chatrecord.h"
#include "afxdialogex.h"


// CChatrecord �Ի���

IMPLEMENT_DYNAMIC(CChatrecord, CDialogEx)

CChatrecord::CChatrecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatrecord::IDD, pParent)
	, myrecord(_T(""))
{

}

CChatrecord::~CChatrecord()
{
	
}

void CChatrecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myrecord);
}


BEGIN_MESSAGE_MAP(CChatrecord, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, OnCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


apple banana ;

// CChatrecord ��Ϣ�������
BOOL CChatrecord::OnInitDialog()
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

	m_caption =(CString)"��(" +  theApp.friendid + (CString)") " + theApp.friendname + (CString)" �����¼";
	SetWindowText(m_caption);//���öԻ������
	CString mm = (CString)"10000" ;
	if(theApp.friendid == mm)  //Ⱥ�ļ�¼
	{
		CString strFilePath = TEXT("");
		strFilePath.Format(TEXT("../MyChattingroom_client/����Ⱥ�������¼.txt"));
		CFile infile;

		if(!infile.Open((LPCTSTR)strFilePath, CFile::modeRead))
		{
			return FALSE ;
		}		
		
		int temper = 1;
		while(temper == 1)
		{
			char message[1024] ;
			if(infile.Read((char*)&banana,sizeof(banana)))
			{
				myrecord += banana.mymesssage ;
				
			}
			else
			{
				infile.Close();
				temper = 0 ;
				UpdateData(false);
			}
		}
	}
	else   //˽�ļ�¼
	{
		
		CString strFilePath = TEXT("");
		strFilePath.Format(TEXT("../MyChattingroom_client/%s��%s�������¼.txt"), 
			(LPCTSTR)theApp.m_username, (LPCTSTR)theApp.friendname);
		CFile infile;
		
		if(!infile.Open((LPCTSTR)strFilePath, CFile::modeRead))
		{
			return FALSE ;
		}
		try
		{
			BOOL bTemper = TRUE;
			while(bTemper == TRUE)
			{
				char message[1024] ;
				if(infile.Read((char*)&banana,sizeof(banana)))
				{
					myrecord+= banana.mymesssage ;
					
				}
				else
				{
					infile.Close();
					bTemper = FALSE ;
					UpdateData(false);
				}
			}
		}
		catch (CFileException* e)
		{
			    
			char szBuf[MAX_PATH] ={0};
			e->GetErrorMessage((LPTSTR)szBuf,MAX_PATH);
			CString strout = (CString)szBuf ;
			MessageBox(strout);
			return FALSE ;
			
		}
		
		
	}


	return TRUE ;
}


HBRUSH CChatrecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CChatrecord::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();   
}



void CChatrecord::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::PostNcDestroy();

	delete theApp.cchat ;
	theApp.cchat = NULL ;
}
