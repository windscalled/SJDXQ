// TcpSocket.cpp : ʵ���ļ�
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


// CTcpSocket ��Ա����
void CTcpSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	case 4:   //��¼
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
	case 5:   //ע��
		temp += (CString)"��ϲ��ע��ɹ�!�˺�Ϊ��" + (CString)Msg.id;
		
		theApp.log->myuserstr = Msg.id;
		theApp.log->myuserstr = theApp.log->myuserstr.Left(6);
		theApp.log->UpdateData(false);
		AfxMessageBox(temp);
		break;
	case 6:   //��ȡ�ܱ�����
		if(Msg.flag != 0)
		{
			theApp.modify_tag = 1;
			theApp.m_question = Msg.question;
			
		}
	    SendMessage(theApp.mod->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 7:   //��ȡ����
		if(Msg.flag != 0)
		{
			theApp.modify_tag = 1;
			theApp.m_password = Msg.password;
		}
		SendMessage(theApp.ques->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 8:  //�ж��Ƿ�����ļ�
		
		tempp = (CString)"���ĺ��ѣ�"+ Msg.nicname+(CString)"���㷢����һ���ļ����Ƿ���գ�\n" ;	
		//�����Ի������û�ѡ���Ƿ�����ļ�
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
			if (IDOK== fileDlg.DoModal())//���ѡ���˱���·��
			{ 	
				CString strFileName = fileDlg.GetPathName();   //�ļ�·��  
				CString strSaveFileFolder =strFileName.Left(strFileName.GetLength()-fileDlg.GetFileName().GetLength()-1);//�õ��ļ���·��	
			    
				theApp.recve->saveFilePath =strFileName;
      			theApp.recve->saveFileFolder =strSaveFileFolder;
				
			}
			else
			{
				AfxMessageBox(_T("�ļ����浽Ĭ��Ŀ¼: d:\\FILE"));
				((CReceivefile*)(AfxGetApp()->m_pMainWnd))->saveFilePath = _T("D:\\FILE\\")+(CString)Msg.message;
			} 
			
			//��ʾ�ļ�����·��
			theApp.recve->SetDlgItemText(IDC_STATIC_road,_T("�ļ�·��: ")+fileDlg.GetPathName());
		    theApp.recve->CreateReceiveSoket();   //���������ļ��׽���
			DWORD dwFileSize = Msg.m_dwFileSize;     //ȡ���ļ���С
			CString m_strFileSize;
			m_strFileSize.Format(_T("%d KB"),dwFileSize/1024);//ת��CString

			//�ļ���С��ֵ�������ڱ���m_dwFileSize
			theApp.recve->m_dwFileSize =dwFileSize;
			//��ʾ�ļ���С
			theApp.recve->SetDlgItemText(IDC_STATIC_sizer,_T("�ļ���С: ")+m_strFileSize);
			//��ʾ�ļ�����
			theApp.recve->SetDlgItemText(IDC_STATIC_nam,_T("�ļ�����: ")+(CString)Msg.message);

			theApp.recve->Invalidate() ;
			//���ý�������Χ
			theApp.recve->progress1.SetRange32(0,Msg.m_dwFileSize);

			Msg.flag = 1 ;   //ͬ�����
			Msg.type = 9 ;
			strcpy_s(Msg.IP,theApp.mylocalip) ;
			theApp.mytcp->Send(&Msg,sizeof(Msg));//�������ļ�

		}
		else if(nType==IDCANCEL)
		{
			Msg.flag = 0 ;   //�ܾ�����
			Msg.type = 9 ;
			theApp.mytcp->Send(&Msg,sizeof(Msg));//�������ļ�

		} 
		break;
	case 9:  //�Է��Ƿ�����ļ�
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

void CTcpSocket::ChatUpdateMsg(CHead Msg)//������Ϣ
{
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S  ");

	CString temp;
	temp = (CString)"(" + (CString)Msg.fromID + (CString)") " + (CString)Msg.nicname;
	if(strcmp(Msg.toID,(CStringA)"10000") == 0) //Ⱥ��
	{
		map<CString,CChat*>::iterator it = theApp.m_usermessage.find((CString)"10000");	
		if(it != theApp.m_usermessage.end())   //�Ƿ�������Ϣ��Դ�������������Ѿ����ˣ�������Ϣ�����û�У����ȴ򿪶Ի���
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
			c->othername =(CString)"���ǵ�����Ⱥ";
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
	else  //˽��
	{
		map<CString,CChat*>::iterator it = theApp.m_usermessage.find((CString)Msg.fromID);	
		if(it != theApp.m_usermessage.end())   //�Ƿ�������Ϣ��Դ�������������Ѿ����ˣ�������Ϣ�����û�У����ȴ򿪶Ի���
		{
			it->second->UpdateData(true);
			record = strTime + temp + (CString)"\r\n    " + (CString)Msg.message + (CString)"\r\n";
			it->second->m_recmessage += record;
			it->second->UpdateData(false);
			it->second->m_recmsgctr.LineScroll(it->second->m_recmsgctr.GetLineCount()-1);
			it->second->m_recmessage = "" ;
			strcpy_s(pie.mymesssage,(CStringA)record) ;  //��Ϣ
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
			strcpy_s(pie.mymesssage,(CStringA)record) ;  //��Ϣ
		}

		//���ļ�
		ofstream outfile;
		char repair[200];
		char tempname[100];
		strcpy_s(tempname,(CStringA)theApp.m_username) ; //�ҵ��ǳ�
		sprintf(repair,"../MyChattingroom_client/%s��%s�������¼.txt",tempname,Msg.nicname);
		outfile.open(repair,ios::binary | ios::app);
		outfile.write((char*)&pie,sizeof(pie));
		outfile.close();
	}
	record = "" ;
	
}

void CTcpSocket::UpdateListCtrl(CHead Msg)//���������û��б�
{
	if(Msg.type == 0)//�пͻ����˳�
	{
		theApp.m_clientmessage.erase((CString)Msg.id);
		theApp.m_client.erase((CString)Msg.id);
	}
	else if(Msg.type == 1)//�пͻ��˼���
	{
		//��������û�����Ϣ
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
	else if(Msg.type == 2)//��������
	{
		theApp.m_clientmessage.insert(make_pair((CString)Msg.id,(CString)Msg.IP));
		theApp.m_client.insert(make_pair((CString)Msg.id,(CString)Msg.nicname));
	}

	map<CString,CString>::iterator it; //���������û��б�
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

	//���ø�����ʾ
	theApp.active->mylistcontrol.SetFocus();
	theApp.active->mylistcontrol.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);  //����״̬
	theApp.active->mylistcontrol.EnsureVisible(0,FALSE) ;

}
