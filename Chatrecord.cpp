// Chatrecord.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Chatrecord.h"
#include "afxdialogex.h"


// CChatrecord 对话框

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

// CChatrecord 消息处理程序
BOOL CChatrecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strPath = TEXT("../../picture/4.bmp");
	if(m_image1.Load((LPCTSTR)strPath) != S_OK) //根据图片路径加载图片 
	{
		AfxMessageBox(TEXT("Load Error!"));
		return FALSE;
	}
	HBITMAP hBit = m_image1;
	CBitmap cBit;
	m_Br2.DeleteObject() ; //用来初始化
	CBitmap* pBit = cBit.FromHandle(hBit);
	m_Br2.CreatePatternBrush(pBit);
	cBit.DeleteObject();

	m_caption =(CString)"与(" +  theApp.friendid + (CString)") " + theApp.friendname + (CString)" 聊天记录";
	SetWindowText(m_caption);//设置对话框标题
	CString mm = (CString)"10000" ;
	if(theApp.friendid == mm)  //群聊记录
	{
		CString strFilePath = TEXT("");
		strFilePath.Format(TEXT("../MyChattingroom_client/讨论群的聊天记录.txt"));
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
	else   //私聊记录
	{
		
		CString strFilePath = TEXT("");
		strFilePath.Format(TEXT("../MyChattingroom_client/%s和%s的聊天记录.txt"), 
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

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CChatrecord::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();   
}



void CChatrecord::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PostNcDestroy();

	delete theApp.cchat ;
	theApp.cchat = NULL ;
}
