// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Login.h"
#include "afxdialogex.h"

// CLogin 对话框

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
	// TODO:  在此添加额外的初始化
	for(int i=0;i!=vec.size();i++)
	{
		myuserctr.AddString((CString)vec[i].c_str());
	}
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
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind ('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
	sPath=sPath.Left(nPos);
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left(nPos);
	sPath+=filename;
	
}


void CLogin::Read() //将record.txt文件内容读到vector容器中
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


void CLogin::Write()//将vector容器内容读到record.txt文件中
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



void CLogin::OnBnClickedregist()   //注册
{
	// TODO: 在此添加控件通知处理程序代码
	CRegist m_register;
	if(m_register.DoModal() != IDOK)
	{
		return;
	}
		
	if(tcp_tag == 0)
	{
		
		if(theApp.search != 1)
		{
			CString st("QQ客户端查找服务器");
			theApp.myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//发送广播
			MessageBox(_T("未搜索到服务器！"));
			return;
		}

		theApp.sereverip = theApp.myudp->s;//服务器IP
		theApp.mytcp = new CTcpSocket();
		if(!theApp.mytcp)
		{
			MessageBox(_T("内存不足！"));
			return;
		}
		else if(!theApp.mytcp->Create())
		{

			MessageBox(_T("创建套接字失败！"));
			
			return;
		}
		else if(!theApp.mytcp->Connect(theApp.sereverip,1003)) 
		{		
			MessageBox(_T("连接服务器失败?"));
			return;
		}
		//theApp.mytcp->Init();
		tcp_tag = 1;
	}
	CHead temp;
	CString only(theApp.mylocalip);
	only += "00000";
	temp.type = 5;		//把注册信息发送给服务器
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(myuserstr == ""||mypassstr == "")
	{
		return;
	}	
	else if(myuserstr.GetLength()!=6)
	{
		MessageBox(_T("对不起，您的账号输入有误！"));
		return;
	}
	if(tcp_tag == 0)
	{
		if(theApp.search != 1)
		{
			CString st("QQ客户端查找服务器");
			theApp.myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//发送广播
			MessageBox(_T("未搜寻到服务器！"),_T("QQ"));
			return;
		}
		theApp.sereverip = theApp.myudp->s;//服务器IP
		theApp.mytcp = new CTcpSocket();
		if(!theApp.mytcp)
		{
			MessageBox(_T("内存不足！"));
			return;
		}
		else if(!theApp.mytcp->Create())
		{
			MessageBox(_T("创建套接字失败！"));
			return;
		}
		else if(!theApp.mytcp->Connect(theApp.sereverip,1003)) 
		{
			MessageBox(_T("连接服务器失败"));
			return;
		}
		
		tcp_tag = 1;
	}
	CHead temp;
	temp.type = 4;		//把用户信息发送给服务器
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy(temp.id,(CStringA)myuserstr);
	strcpy_s(temp.password,(CStringA)mypassstr);
	strcpy(temp.fromID,(CStringA)myuserstr);

	theApp.mytcp->Send(&temp,sizeof(temp));
}

LRESULT CLogin::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.tag == 1)  //帐号密码匹配成功
	{
		//将新的帐号加入容器中并存储在本地文件
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

		//发送登录信息
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
		MessageBox(_T("帐号已登录！若非您本人操作，请及时修改密码！"),_T("QQ"));
	}
	else 
	{
		MessageBox(_T("账号或密码错误！"),_T("QQ"));
	}
		
	return TRUE;
}


void CLogin::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;
	}
	/*if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}*/
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CLogin::OnBnClickedpassword()   //忘记密码
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.mod = new CModify() ;
	theApp.mod->Create(IDD_DIALOG2,NULL) ;
	theApp.mod->ShowWindow(SW_SHOW) ;
	
}


void CLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //取客户区大小 

	CString str("C++聊天室") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(280,(LPTSTR)"华文行楷");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //设置透明背景
	dc.TextOut(rect.left+130,rect.top+20,str) ;
	dc.SelectObject(oldfont1);


	CFont font2;
	dc.SetTextColor(RGB(30,144,255)) ;
	//font2.CreateFont(56,30,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,OUT_DEVICE_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("隶书"));
	font2.CreatePointFont(210,(LPTSTR)"华文行楷");
	CFont *oldfont2 = dc.SelectObject(&font2);
	dc.SetBkMode(TRANSPARENT) ;   //设置透明背景
	dc.SelectObject(oldfont2);
}


BOOL CLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

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
