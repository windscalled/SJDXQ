// TcpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "TcpSocket.h"


apple pie ;

// CTcpSocket

CTcpSocket::CTcpSocket()
{
}

CTcpSocket::~CTcpSocket()
{
}

CString tempp ;
int nType ;


// CTcpSocket 成员函数
void CTcpSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int n;
	CString temp;
	CHead Msg;
	n=Receive(&Msg,sizeof(Msg));
	switch(Msg.type)
	{
	case 0:
	case 1:
	case 2:
		UpdateListCtrl(Msg);
		break;
	case 3:
		ChatUpdateMsg(Msg);
		break;
	case 4:   //登录
		if(Msg.flag == 1)
		{
			theApp.tag = 1;
			theApp.m_username = Msg.nicname;
			theApp.m_userid = Msg.id ;
		}
		else if(Msg.flag == 5)
		{
			theApp.tag = 5;
		}
		SendMessage(theApp.log->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 5:   //注册
		temp += (CString)"恭喜您注册成功!账号为：" + (CString)Msg.id;
		
		theApp.log->myuserstr = Msg.id;
		theApp.log->myuserstr = theApp.log->myuserstr.Left(6);
		theApp.log->UpdateData(false);
		AfxMessageBox(temp);
		break;
	case 6:   //获取密保问题
		if(Msg.flag != 0)
		{
			theApp.modify_tag = 1;
			theApp.m_question = Msg.question;
			
		}
	    SendMessage(theApp.mod->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 7:   //获取密码
		if(Msg.flag != 0)
		{
			theApp.modify_tag = 1;
			theApp.m_password = Msg.password;
		}
		SendMessage(theApp.ques->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 8:  //判断是否接收文件
		
		tempp = (CString)"您的好友："+ Msg.nicname+(CString)"给你发送了一个文件，是否接收？\n" ;	
		//弹出对话框让用户选择是否接收文件
		if(dlg1.m_hWnd == 0)
		{
			dlg1.Create(IDD_DIALOG5,theApp.recve);
			
		}	
		nType = AfxMessageBox(tempp,MB_OKCANCEL|MB_ICONQUESTION);
		if (nType==IDOK)
		{   
			dlg1.ShowWindow(SW_SHOW);	
			CFileDialog fileDlg(FALSE,NULL,(CString)Msg.message);
			CFile file;
			if (IDOK== fileDlg.DoModal())//如果选择了保存路径
			{ 	
				CString strFileName = fileDlg.GetPathName();   //文件路径  
				CString strSaveFileFolder =strFileName.Left(strFileName.GetLength()-fileDlg.GetFileName().GetLength()-1);//得到文件夹路径	
			    
				theApp.recve->saveFilePath =strFileName;
      			theApp.recve->saveFileFolder =strSaveFileFolder;
				
			}
			else
			{
				AfxMessageBox(_T("文件保存到默认目录: d:\\FILE"));
				((CReceivefile*)(AfxGetApp()->m_pMainWnd))->saveFilePath = _T("D:\\FILE\\")+(CString)Msg.message;
			} 
			
			//显示文件保存路径
			theApp.recve->SetDlgItemText(IDC_STATIC_road,_T("文件路径: ")+fileDlg.GetPathName());
		    theApp.recve->CreateReceiveSoket();   //创建接受文件套接字
			DWORD dwFileSize = Msg.m_dwFileSize;     //取得文件大小
			CString m_strFileSize;
			m_strFileSize.Format(_T("%d KB"),dwFileSize/1024);//转成CString

			//文件大小赋值给主窗口变量m_dwFileSize
			theApp.recve->m_dwFileSize =dwFileSize;
			//显示文件大小
			theApp.recve->SetDlgItemText(IDC_STATIC_sizer,_T("文件大小: ")+m_strFileSize);
			//显示文件名称
			theApp.recve->SetDlgItemText(IDC_STATIC_nam,_T("文件名称: ")+(CString)Msg.message);

			theApp.recve->Invalidate() ;
			//设置进度条范围
			theApp.recve->progress1.SetRange32(0,Msg.m_dwFileSize);

			Msg.flag = 1 ;   //同意接受
			Msg.type = 9 ;
			strcpy_s(Msg.IP,theApp.mylocalip) ;
			theApp.mytcp->Send(&Msg,sizeof(Msg));//请求发送文件

		}
		else if(nType==IDCANCEL)
		{
			Msg.flag = 0 ;   //拒绝接受
			Msg.type = 9 ;
			theApp.mytcp->Send(&Msg,sizeof(Msg));//请求发送文件

		} 
		break;
	case 9:  //对方是否接收文件
		if(Msg.flag == 1)
		{
			theApp.otherlocalip = Msg.IP ;
			theApp.sendFile->sendFile();
		}
		break;
	default:
		break;
	}
	CSocket::OnReceive(nErrorCode);
}

void CTcpSocket::ChatUpdateMsg(CHead Msg)//接收消息
{
	CTime time;
	time = CTime::GetCurrentTime();  //获取现在时间
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S  ");

	CString temp;
	temp = (CString)"(" + (CString)Msg.fromID + (CString)") " + (CString)Msg.nicname;
	if(strcmp(Msg.toID,(CStringA)"10000") == 0) //群聊
	{
		map<CString,CChat*>::iterator it = theApp.m_usermessage.find((CString)"10000");	
		if(it != theApp.m_usermessage.end())   //是否已与消息来源打开了聊天框，如果已经打开了，则处理消息；如果没有，则先打开对话框
		{
			
			it->second->UpdateData(true);
			record = strTime + temp + (CString)"\r\n    " + (CString)Msg.message + (CString)"\r\n";
			it->second->m_recmessage += record ;
			it->second->UpdateData(false);
			it->second->m_recmessage = "" ;
			it->second->m_recmsgctr.LineScroll(it->second->m_recmsgctr.GetLineCount()-1);	
			
		}
		else
		{
			CChat *c;
			c = new CChat;
			c->mytoid = (CString)"10000";
			c->othername =(CString)"我们的讨论群";
			c->Create(IDD_CHAT,theApp.active);
			c->ShowWindow(SW_SHOW);
			theApp.m_usermessage.insert(make_pair((CString)"10000",c));
			record = strTime + temp + (CString)"\r\n    " + (CString)Msg.message+ (CString)"\r\n" ;
			c->m_recmessage += record ;
			c->UpdateData(false);
			c->m_recmsgctr.LineScroll(c->m_recmsgctr.GetLineCount()-1);
			c->m_recmessage = "";
		}

	}
	else  //私聊
	{
		map<CString,CChat*>::iterator it = theApp.m_usermessage.find((CString)Msg.fromID);	
		if(it != theApp.m_usermessage.end())   //是否已与消息来源打开了聊天框，如果已经打开了，则处理消息；如果没有，则先打开对话框
		{
			it->second->UpdateData(true);
			record = strTime + temp + (CString)"\r\n    " + (CString)Msg.message + (CString)"\r\n";
			it->second->m_recmessage += record;
			it->second->UpdateData(false);
			it->second->m_recmsgctr.LineScroll(it->second->m_recmsgctr.GetLineCount()-1);
			it->second->m_recmessage = "" ;
			strcpy_s(pie.mymesssage,(CStringA)record) ;  //消息
		}
		else
		{
			CChat *c;
			c = new CChat;
			c->mytoid = Msg.fromID;
			c->othername = Msg.nicname;
			c->Create(IDD_CHAT,theApp.active);
			c->ShowWindow(SW_SHOW);
			theApp.m_usermessage.insert(make_pair(Msg.fromID,c));
			record = strTime + temp + (CString)"\r\n    " + (CString)Msg.message+ (CString)"\r\n";
			c->m_recmessage += record ;
			c->UpdateData(false);
			c->m_recmsgctr.LineScroll(c->m_recmsgctr.GetLineCount()-1);
			c->m_recmessage = "" ;
			strcpy_s(pie.mymesssage,(CStringA)record) ;  //消息
		}

		//打开文件
		ofstream outfile;
		char repair[200];
		char tempname[100];
		strcpy_s(tempname,(CStringA)theApp.m_username) ; //我的昵称
		sprintf(repair,"../MyChattingroom_client/%s和%s的聊天记录.txt",tempname,Msg.nicname);
		outfile.open(repair,ios::binary | ios::app);
		outfile.write((char*)&pie,sizeof(pie));
		outfile.close();
	}
	record = "" ;
	
}

void CTcpSocket::UpdateListCtrl(CHead Msg)//更新在线用户列表
{
	if(Msg.type == 0)//有客户端退出
	{
		theApp.m_clientmessage.erase((CString)Msg.id);
		theApp.m_client.erase((CString)Msg.id);
	}
	else if(Msg.type == 1)//有客户端加入
	{
		//添加在线用户的信息
		theApp.m_clientmessage.insert(make_pair((CString)Msg.id,(CString)Msg.IP)); 
		theApp.m_client.insert(make_pair((CString)Msg.id,(CString)Msg.nicname));

		CHead temp;
		temp.type = 2;
		strcpy(temp.fromID,(CStringA)theApp.m_userid);
		strcpy(temp.toID,Msg.fromID);
		strcpy_s(temp.nicname,(CStringA)theApp.m_username);
		strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
		strcpy(temp.id,(CStringA)theApp.m_userid);
		theApp.mytcp->Send(&temp,sizeof(temp));
	}
	else if(Msg.type == 2)//我已在线
	{
		theApp.m_clientmessage.insert(make_pair((CString)Msg.id,(CString)Msg.IP));
		theApp.m_client.insert(make_pair((CString)Msg.id,(CString)Msg.nicname));
	}

	map<CString,CString>::iterator it; //更新在线用户列表
	int i;
	theApp.active->mylistcontrol.DeleteAllItems();
	for(it = theApp.m_clientmessage.begin(),i = 0;it != theApp.m_clientmessage.end();it++,i++)
	{
		theApp.active->mylistcontrol.InsertItem(i, it->first);
		theApp.active->mylistcontrol.SetItemText(i,1,theApp.m_client.find(it->first)->second);
		theApp.active->mylistcontrol.SetItemText(i, 2, it->second);
	}

	
	int n = theApp.active->mylistcontrol.GetItemCount() ;

	for(int i = 0;i <= n-1;i++)
	{
		CString b = theApp.active->mylistcontrol.GetItemText(i,0) ;
		if(theApp.m_userid == b)
		{
			n = i ;
			break;
		}
	}

	theApp.active->mylistcontrol.DeleteItem(n);
	theApp.active->mylistcontrol.InsertItem(0,theApp.m_userid) ;
	theApp.active->mylistcontrol.SetItemText(0,1,theApp.m_username);
	theApp.active->mylistcontrol.SetItemText(0,2,theApp.mylocalip);

	theApp.active->mylistcontrol.SetItemData(0,1);

	//设置高亮显示
	theApp.active->mylistcontrol.SetFocus();
	theApp.active->mylistcontrol.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);  //设置状态
	theApp.active->mylistcontrol.EnsureVisible(0,FALSE) ;

}
