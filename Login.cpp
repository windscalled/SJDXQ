// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Login.h"
#include "afxdialogex.h"

// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, myuserstr(_T(""))
	, mypassstr(_T(""))
{
	theApp.log = this;
	tcp_tag = 0;
	//GetPath(sPath,_T("\\res\\record.txt"));
	sPath = (CString)"../MyChattingroom_client/res/record.txt" ;
	Read();
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, mypassstr);
	DDX_Control(pDX, IDC_COMBO1, myuserctr);
	DDX_CBString(pDX, IDC_COMBO1, myuserstr);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_regist, &CLogin::OnBnClickedregist)
	ON_BN_CLICKED(IDC_denglu, &CLogin::OnBnClickeddenglu)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLogin::OnCbnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_password, &CLogin::OnBnClickedpassword)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i=0;i!=vec.size();i++)
	{
		myuserctr.AddString((CString)vec[i].c_str());
	}
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

	HBITMAP hbit_button1, hbit_button2,hbit_button3;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/regist.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/password.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/deng.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	
	((CButton*)GetDlgItem(IDC_regist))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_password))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_denglu))->SetBitmap(hbit_button3) ;
	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	
}

void CLogin::GetPath(CString &sPath,CString filename)
{
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);//���ظõ�ǰӦ�ó���*.exe��ȫ·��
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind ('\\');//ReverseFind()��һ���ϴ���ַ����д�ĩ�˿�ʼ����ĳ���ַ�
	sPath=sPath.Left(nPos);
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left(nPos);
	sPath+=filename;
	
}


void CLogin::Read() //��record.txt�ļ����ݶ���vector������
{
	fstream infile(sPath,fstream::in);
	if(infile)
	{
		string temp;
		while(!infile.eof())
		{
			infile>>temp;
			vec.push_back(temp);
		}
	}
	infile.close();
}


void CLogin::Write()//��vector�������ݶ���record.txt�ļ���
{
	fstream out(sPath,fstream::out);
	if(out)
	{
		out<<vec[0];
		for(int i = 1;i != vec.size();i++)
		{
			out<<endl<<vec[i];
		}		
	}
	out.close();
}



void CLogin::OnBnClickedregist()   //ע��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRegist m_register;
	if(m_register.DoModal() != IDOK)
	{
		return;
	}
		
	if(tcp_tag == 0)
	{
		
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
		tcp_tag = 1;
	}
	CHead temp;
	CString only(theApp.mylocalip);
	only += "00000";
	temp.type = 5;		//��ע����Ϣ���͸�������
	strcpy_s(temp.message,(CStringA)only);
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy_s(temp.password,(CStringA)m_register.mypassword);
	strcpy_s(temp.question,(CStringA)m_register.question);
	strcpy_s(temp.answer,(CStringA)m_register.answer);
	strcpy_s(temp.nicname,(CStringA)m_register.mynicname);
	theApp.mytcp->Send(&temp,sizeof(temp));
}


void CLogin::OnBnClickeddenglu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(myuserstr == ""||mypassstr == "")
	{
		return;
	}	
	else if(myuserstr.GetLength()!=6)
	{
		MessageBox(_T("�Բ��������˺���������"));
		return;
	}
	if(tcp_tag == 0)
	{
		if(theApp.search != 1)
		{
			CString st("QQ�ͻ��˲��ҷ�����");
			theApp.myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//���͹㲥
			MessageBox(_T("δ��Ѱ����������"),_T("QQ"));
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
			MessageBox(_T("���ӷ�����ʧ��"));
			return;
		}
		
		tcp_tag = 1;
	}
	CHead temp;
	temp.type = 4;		//���û���Ϣ���͸�������
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy(temp.id,(CStringA)myuserstr);
	strcpy_s(temp.password,(CStringA)mypassstr);
	strcpy(temp.fromID,(CStringA)myuserstr);

	theApp.mytcp->Send(&temp,sizeof(temp));
}

LRESULT CLogin::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.tag == 1)  //�ʺ�����ƥ��ɹ�
	{
		//���µ��ʺż��������в��洢�ڱ����ļ�
		string t((CStringA)myuserstr);  
		int k = 0;
		for(int i =0;i!=vec.size();i++)
		{
			if(t == vec[i])
			{
				k = 1;
				break;
			}
		}
		if( k == 0)
		{
			vec.push_back((string)(CStringA)myuserstr);
		}	
		Write();

		//���͵�¼��Ϣ
		CHead temp;  
		strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
		strcpy(temp.id,(CStringA)myuserstr);
		strcpy_s(temp.password,(CStringA)mypassstr);
		strcpy(temp.fromID,(CStringA)myuserstr);
		temp.type = 1;
		strcpy_s(temp.nicname,(CStringA)theApp.m_username);
		theApp.mytcp->Send(&temp,sizeof(temp));
		CDialogEx::OnOK();
	}
	else if(theApp.tag == 5)
	{
		MessageBox(_T("�ʺ��ѵ�¼�����������˲������뼰ʱ�޸����룡"),_T("QQ"));
	}
	else 
	{
		MessageBox(_T("�˺Ż��������"),_T("QQ"));
	}
		
	return TRUE;
}


void CLogin::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
	}
	/*if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CLogin::OnBnClickedpassword()   //��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.mod = new CModify() ;
	theApp.mod->Create(IDD_DIALOG2,NULL) ;
	theApp.mod->ShowWindow(SW_SHOW) ;
	
}


void CLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������

	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //ȡ�ͻ�����С 

	CString str("C++������") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(280,(LPTSTR)"�����п�");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //����͸������
	dc.TextOut(rect.left+130,rect.top+20,str) ;
	dc.SelectObject(oldfont1);


	CFont font2;
	dc.SetTextColor(RGB(30,144,255)) ;
	//font2.CreateFont(56,30,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("����"));
	font2.CreatePointFont(210,(LPTSTR)"�����п�");
	CFont *oldfont2 = dc.SelectObject(&font2);
	dc.SetBkMode(TRANSPARENT) ;   //����͸������
	dc.SelectObject(oldfont2);
}


BOOL CLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBnClickeddenglu();
			return TRUE ;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
