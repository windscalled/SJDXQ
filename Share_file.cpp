// Share_file.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Share_file.h"
#include "afxdialogex.h"


// CShare_file �Ի���

IMPLEMENT_DYNAMIC(CShare_file, CDialog)

CShare_file::CShare_file(CWnd* pParent /*=NULL*/)
	: CDialog(CShare_file::IDD, pParent)
{

}

CShare_file::~CShare_file()
{
}

void CShare_file::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_BUTTON5, m_back);
	//DDX_Control(pDX, IDCANCEL, m_cancle);
}


BEGIN_MESSAGE_MAP(CShare_file, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON5, &CShare_file::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CShare_file::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CShare_file::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CShare_file::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LIST1, &CShare_file::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON8, &CShare_file::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &CShare_file::OnBnClickedButton4)
END_MESSAGE_MAP()


// CShare_file ��Ϣ�������


BOOL CShare_file::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	bFlag = FALSE ;
	HBITMAP hbit_button1, hbit_button2,hbit_button3,hbit_button4,hbit_button5,hbit_button6,hbit_button7,hbit_button8;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/up.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/down.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/delete.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/exit (2).bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button5 = (HBITMAP)::LoadImage(NULL,"../../picture/new_file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button6 = (HBITMAP)::LoadImage(NULL,"../../picture/return_f.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button8 = (HBITMAP)::LoadImage(NULL,"../../picture/re_name.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON6))->SetBitmap(hbit_button4) ;
	((CButton*)GetDlgItem(IDC_BUTTON5))->SetBitmap(hbit_button6) ;
	((CButton*)GetDlgItem(IDC_BUTTON4))->SetBitmap(hbit_button5) ;
	((CButton*)GetDlgItem(IDC_BUTTON8))->SetBitmap(hbit_button8) ;

	ConnectFtp() ;
	if(!bFlag)
	{
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON6))->EnableWindow(TRUE) ;
		((CButton*)GetDlgItem(IDC_BUTTON5))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON8))->EnableWindow(FALSE) ;
		AfxMessageBox((CString)szErr+"   ���˳���");
		return FALSE ;
	}

	UpdateDir() ;
	m_back.EnableWindow(false);

	

	//m_cancle.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CShare_file::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CShare_file::ConnectFtp()  //���Ӵ洢������
{
	
	CString Ftp_IP = theApp.sereverip ;    //ftp������IP��ַ
	int nPort = 21 ;                     //�˿ں�
	CString str_user = "anonymous" ;     //����
	CString str_pass = "" ;              //����Ϊ��

	if (Ftp_IP.IsEmpty())
	{
		AfxMessageBox(_T("IP��ַΪ�գ�"));
		return;
	}
	if (str_user.IsEmpty())
	{
		return;
	}
	//����һ��Internet�Ự
	pInternetSession= new CInternetSession("MR",INTERNET_OPEN_TYPE_PRECONFIG);

	try
	{
		//����Internet�Ự����pInternetSession��һ��FTP����
		pFtpConnection=pInternetSession->GetFtpConnection(Ftp_IP,str_user,str_pass,nPort);
		bFlag = TRUE ;
	}
	catch (CInternetException* pEx)
	{
		pEx->GetErrorMessage(szErr,1024);
		
		pEx->Delete();
	}

}


void CShare_file::OnBnClickedButton5()   //������һ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);

	int pos;
	//���ַ�����ȡ�ķ��������һ��Ŀ¼
	pos=strdir.ReverseFind('/');
	strdir=strdir.Left(pos);

	pInternetSession->Close();//�رշ����ĶԻ�
	this->ConnectFtp();//�������ӱ��ֳ����Ự
	pFtpConnection->SetCurrentDirectory(strdir);
	theApp.str_LoadPath = strdir ;
	this->UpdateDir();//����Ŀ¼�б�

}


void CShare_file::OnBnClickedButton1()   //�ϴ���Դ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	CString strname;
	//�������򿪡��Ի���
	CFileDialog file(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�����ļ�(*.*)|*.*|",this);
	if (file.DoModal()==IDOK)
	{
		str=file.GetPathName();
		strname=file.GetFileName();
	}
	if (bFlag)   //������
	{
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);
		//�ϴ��ļ�
		BOOL bput=pFtpConnection->PutFile((LPCTSTR)str,(LPCTSTR)strname);
		if (bput)
		{
			pInternetSession->Close();//�رջỰ
			this->ConnectFtp();//�������ӱ��ֳ����Ự
			pFtpConnection->SetCurrentDirectory(strdir);
			this->UpdateDir();//����Ŀ¼�б�

			//�����ϴ���Դ��Ϣ
			CTime time=time.GetCurrentTime();
			CString str=time.Format("%Y-%m-%d %X");
			CString str2 = "�ϴ��ļ���"+strname ;
			CHead temp;  
			strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
			strcpy(temp.id,(CStringA)theApp.m_userid);
			strcpy_s(temp.password,(CStringA)"");
			strcpy(temp.fromID,(CStringA)"");
			temp.type = 10;
			strcpy(temp.message,str) ;
			strcpy_s(temp.nicname,(CStringA)theApp.m_username);
			strcpy_s(temp.question,str2);
			theApp.mytcp->Send(&temp,sizeof(temp));

			AfxMessageBox(_T("�ϴ��ɹ���"));
		}
	}
}


void CShare_file::OnBnClickedButton2()   //����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString selfile;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//��ȡ�û�ѡ��Ҫ���ص���Դ��
	selfile = selfile.Right(selfile.GetLength()-13);
	if (!selfile.IsEmpty())
	{
		//���������Ϊ���Ի���
		CFileDialog file(FALSE,NULL,selfile,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�����ļ�(*.*)|*.*|",this);
		if (file.DoModal()==IDOK)
		{
			CString strname;
			CString strpath ;
			strname=file.GetFileName();
			strpath = file.GetPathName() ;
			CString strdir;
			pFtpConnection->GetCurrentDirectory(strdir);
			pFtpConnection->GetFile(strname,strpath);//�����ļ���ѡ���ı���λ��
			pInternetSession->Close();//�رշ����ĶԻ�
			this->ConnectFtp();//���ֳ����Ự
			pFtpConnection->SetCurrentDirectory(strdir);
			this->UpdateDir();//����Ŀ¼�б�

			//����������Դ��Ϣ
			CTime time=time.GetCurrentTime();
			CString str=time.Format("%Y-%m-%d %X");
			CString str2 = "�����ļ���"+strname ;
			CHead temp;  
			strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
			strcpy(temp.id,(CStringA)theApp.m_userid);
			strcpy_s(temp.password,(CStringA)"");
			strcpy(temp.fromID,(CStringA)"");
			temp.type = 10;
			strcpy(temp.message,str) ;
			strcpy_s(temp.nicname,(CStringA)theApp.m_username);
			strcpy_s(temp.question,str2);
			theApp.mytcp->Send(&temp,sizeof(temp));

			AfxMessageBox(_T("���سɹ���"));
		}
	}
}


void CShare_file::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //ȡ�ͻ�����С 

	CString str("� �� �� �� �� ��") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(280,(LPTSTR)"�����п�");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //����͸������
	dc.TextOut(rect.left+130,rect.top+20,str) ;
	dc.SelectObject(oldfont1);
}


void CShare_file::UpdateDir()   //���´���
{
	m_listBox.ResetContent();  //�����Ͽ�����

	//��д�������е����ݣ���Ҫ����һ��CFtpFileFind��ʵ��
	CFtpFileFind ftpfind(pFtpConnection);

	//�ҵ���һ���ļ������ļ��У�ͨ��CFtpFileFind::FindFileʵ��
	BOOL bfind=ftpfind.FindFile(NULL);
	while (bfind)
	{
		bfind=ftpfind.FindNextFile();
		CString strname;
		if (!ftpfind.IsDirectory())//�ж���Ŀ¼�����ļ���
		{
			strname=ftpfind.GetFileName();//���ļ����ȡ�ļ���
			m_listBox.AddString("�ļ�         "+strname);
		}
		else
		{
			strname=ftpfind.GetFileName();//������ļ������ȡ�ļ���
			m_listBox.AddString("�ļ���     "+strname);
		}
	}
}

void CShare_file::OnLbnDblclkList1()   //���ļ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString selfile,str;
	// ��ȡ�û�ѡ���Ŀ¼��
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);

	if((str = selfile.Left(13)) == "�ļ�         ")
	{
		return ;
	}

	selfile = selfile.Right(selfile.GetLength()-11);

	if (!selfile.IsEmpty())
	{
		pFtpConnection->Close();   //��ʱ�رշ����ĻỰ���
		this->ConnectFtp();        //�������ӣ�������������ĳ����Ự
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);//���ԭ���Ĺ���Ŀ¼
		strdir+=selfile;//�����µ�Ŀ¼
		theApp.str_LoadPath = theApp.str_LoadPath+ strdir ;
		pFtpConnection->SetCurrentDirectory(theApp.str_LoadPath); //�ı�Ŀ¼����ǰ����Ŀ¼
		this->UpdateDir();         //����Ŀ¼�б�

		m_back.EnableWindow(TRUE);
	}
}


void CShare_file::OnBnClickedButton8()   //�������ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString selfile;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//��ȡ��Դ��
	if(selfile.Find("�ļ���     ") == -1)
	{
		selfile = selfile.Right(selfile.GetLength()-13);
	}
	else
	{
		selfile = selfile.Right(selfile.GetLength()-11);
	}
	
	theApp.ziyuan = selfile ;
	if (!selfile.IsEmpty())
	{
		if(re_name.DoModal()!=IDOK)
		{
			return ;
		}
		theApp.ziyuan = "";
		pFtpConnection->Rename(selfile,theApp.m_rename);//������
		
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);
		pInternetSession->Close();//�رշ����ĻỰ
		this->ConnectFtp();//���ֳ����Ự
		pFtpConnection->SetCurrentDirectory(strdir);
		this->UpdateDir();//����Ŀ¼�б�
	}
}


void CShare_file::OnBnClickedButton4()  //�½��ļ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(re_name.DoModal()!=IDOK)
	{
		return ;
	}

	int flag = pFtpConnection->CreateDirectory(theApp.m_rename);  
	if (!flag)  
	{  
		AfxMessageBox("�����ļ���ʧ�ܣ�");
	} 
	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);
	pInternetSession->Close();//�رշ����ĻỰ
	this->ConnectFtp();//���ֳ����Ự
	pFtpConnection->SetCurrentDirectory(strdir);
	this->UpdateDir();//����Ŀ¼�б�
}


void CShare_file::OnBnClickedButton3()  //ɾ���ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString selfile;
	CString str2 ;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//��ȡ�û�Ҫɾ������Դ��

	if(selfile.Find("�ļ���     ") == -1)
	{
		selfile = selfile.Right(selfile.GetLength()-13);
		if (!selfile.IsEmpty())
		{
			//����ɾ�������
			if (AfxMessageBox("ȷ��Ҫɾ������ļ���",4+48)==6)
			{
				pFtpConnection->Remove(selfile);//ɾ�����ļ�				
				str2 = "ɾ���ļ���"+selfile ;				
			}
		}
	}
	else
	{
		selfile = selfile.Right(selfile.GetLength()-11);
		if (!selfile.IsEmpty())
		{
			//����ɾ�������
			if (AfxMessageBox("ȷ��ɾ�����ļ��У�",4+48)==6)
			{
				int flag = pFtpConnection->RemoveDirectory(selfile);  
				if (!flag)  
				{  
					AfxMessageBox("ɾ��ʧ�ܣ��ļ��б���Ϊ�գ�");
				} 
				str2 = "ɾ���ļ��У�"+selfile ;
				
			}
		}
	}
	//����ɾ����Դ��Ϣ
	CTime time=time.GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %X");
	CHead temp;  
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy(temp.id,(CStringA)theApp.m_userid);
	strcpy_s(temp.password,(CStringA)"");
	strcpy(temp.fromID,(CStringA)"");
	temp.type = 10;
	strcpy(temp.message,str) ;
	strcpy_s(temp.nicname,(CStringA)theApp.m_username);
	strcpy_s(temp.question,str2);
	theApp.mytcp->Send(&temp,sizeof(temp));

	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);
	pInternetSession->Close();//�رշ����ĻỰ
	this->ConnectFtp();//���ֳ����Ự
	pFtpConnection->SetCurrentDirectory(strdir);
	this->UpdateDir();//����Ŀ¼�б�
	
}


//void CShare_file::OnBnClickedButton9()  //�����ļ�
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CFtpFileFind fFinder(pFtpConnection);  
//	if(re_name.DoModal()!=IDOK)
//	{
//		return ;
//	}
//
//	BOOL bFind = fFinder.FindFile(theApp.m_rename);  
//	while (bFind)  
//	{  
//		bFind = fFinder.FindNextFile();  
//
//		//��ǰ�ļ��м��ϲ��ļ���(���Ʒֱ�Ϊ.��..)
//		if (fFinder.IsDots())   
//		{  
//			continue;  
//		}  
//
//		//���ļ���
//		if(fFinder.IsDirectory())   
//		{  
//			CString cstrDirName = fFinder.GetFileName();  
//			CString cstrDirPath = fFinder.GetFilePath(); 
//			continue;  
//		}  
//
//		//�ļ�
//		CString cstrFileName = fFinder.GetFileName();    
//		CString cstrFilePath = fFinder.GetFilePath(); 
//
//		pInternetSession->Close();//�رշ����ĻỰ
//		this->ConnectFtp();//���ֳ����Ự
//		pFtpConnection->SetCurrentDirectory(cstrFilePath);
//		this->UpdateDir();//����Ŀ¼�б�
//	}  
//
//	fFinder.Close();
//}


void CShare_file::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
    DestroyWindow() ;
}


void CShare_file::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PostNcDestroy();

	theApp.str_LoadPath = "" ;
	pInternetSession->Close();//�����Ự
	m_listBox.ResetContent();

	delete theApp.share_file ;
	theApp.share_file = NULL ;
}
