// TcpClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_server.h"
#include "TcpClient.h"


// CTcpClient

CTcpClient::CTcpClient(CPtrList* pList)
	:m_pList(pList)
{
	n_LoginErr = 0 ;
}

CTcpClient::~CTcpClient()
{
}


// CTcpClient ��Ա����

void CTcpClient::OnReceive(int nErrorCode)
{
	int n,tag = 0;
	CHead Msg;
	n=Receive(&Msg,sizeof(Msg));
	switch(Msg.type)
	{
	case 1:    //��¼��Ϣ
		{
			m_strID = Msg.id;
			m_strIp = Msg.IP;
			theApp.m_clientMessage.insert(make_pair(Msg.id,Msg.IP));
			theApp.head.type = 1;
			SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
			break;
		}
	case 4:    //��¼
		{
			//��mysql��ƥ���˺ź�����

			int temp ;
			m_strID = Msg.id;
			m_strIp = Msg.IP;
			theApp.my_mysql();
			char buff[200];
			sprintf(buff,"select * from information where qqnum='%s';",(Msg.id));
			int result = mysql_query(theApp.mysql, buff);
			if(result != 0)
			{
				Msg.flag = 0 ;
			}
			else
			{
				MYSQL_RES* mysql_res;
				MYSQL_FIELD* mysql_field;
				MYSQL_ROW mysql_row;
				unsigned int cols;
				mysql_res = mysql_store_result(theApp.mysql);
				cols = mysql_num_fields(mysql_res);
				if (mysql_res)
				{
					while ((mysql_row = mysql_fetch_row(mysql_res)))
					{

						if(strcmp(mysql_row[2],Msg.password) == 0)
						{
							strcpy(Msg.nicname,mysql_row[0]) ;
							temp = 1 ;
							Msg.flag = 1 ;	
							n_LoginErr = 0 ;
						}
					}
					mysql_free_result(mysql_res);
				}

				if(temp != 1)   //�������
				{
					Msg.flag = 0 ;
				    n_LoginErr = 1 ;
				}
				else
				{
					//�ж��Ƿ��ظ���¼
					map<CString,CString>::iterator it; 
					int i;
					for(it = theApp.m_clientMessage.begin(),i = 0;it != theApp.m_clientMessage.end();it++,i++)
					{
						if(it->first == Msg.id)
						{
							Msg.flag = 5 ;
							n_LoginErr = 1 ;
							break;
						}
					}
				}
			}
			break;
		}
	case 5:   //ע����Ϣ
		{
			//��mysql������˻���������
			m_strIDtemp = Msg.message;
			m_strIp = Msg.IP;
			int IDtemp;
			int tagtemp = 0;
			while(tagtemp == 0)
			{

				srand(time(NULL)) ;
				IDtemp = rand()%900000+100000 ;
				int temp = 1;	    
				sprintf(Msg.id, "%d ",IDtemp);
				theApp.my_mysql();
				char buff[200] ;
				sprintf(buff,"select * from information where qqnum='%s';",Msg.id);
				int result = mysql_query(theApp.mysql, buff);
				if (result != 0)
				{			
					temp = 1 ;//δ�ҵ���ͬ���˺�		
				}
				else
				{		
					NULL;//���˺��ѱ�ע��	
				}

				if(temp == 1)
				{
					int result;
					// û�з��ؽ�����Ĳ���
					theApp.my_mysql();
					char buff[500];
					sprintf(buff,"insert into information values('%s','%s','%s','%s','%s');",Msg.nicname,Msg.id,Msg.password,Msg.question,Msg.answer);

					result = mysql_query(theApp.mysql,buff);
					if (result != 0)
					{
						AfxMessageBox(_T("ע������ʧ�ܣ����飡"));
						AfxMessageBox((CString)mysql_error(theApp.mysql));
					}
					else
					{
						//AfxMessageBox(_T("�û�ע��ɹ�!"));
						tagtemp = 1 ;

					}
				}
				else
				{
					tagtemp = 0;
				}
			}
			break;
		}
	case 6:    //�ж����޴�ID
		{
			m_strIDtemp = Msg.message;
			theApp.my_mysql();
			char buff[200];
			sprintf(buff,"select * from information where qqnum='%s';",(Msg.id));
			mysql_query(theApp.mysql,buff);

			MYSQL_RES* mysql_res;
			MYSQL_FIELD* mysql_field;
			MYSQL_ROW mysql_row;
			unsigned int cols;
			mysql_res = mysql_store_result(theApp.mysql);
			cols = mysql_num_fields(mysql_res);
			if (mysql_res)
			{
				if((mysql_row = mysql_fetch_row(mysql_res)))
				{
					if(strcmp(Msg.id,mysql_row[1])==0)
					{
						strcpy(Msg.question,mysql_row[3]) ;   //���ܱ����ⷢ�͸��ͻ���
						Msg.flag = 1 ;
					}
					else
					{
						Msg.flag = 0;
					}
				}
				else
				{
					Msg.flag = 0;
				}			
				mysql_free_result(mysql_res);
			}
			break;
		}
	case 7:    //�жϻش����Ⲣ�һ�����
		{
			m_strIDtemp = Msg.message;
			theApp.my_mysql();
			char buff[200];
			sprintf(buff,"select * from information where qqnum='%s';",(Msg.id));
			mysql_query(theApp.mysql, buff);

			MYSQL_RES* mysql_res;
			MYSQL_FIELD* mysql_field;
			MYSQL_ROW mysql_row;
			unsigned int cols;
			mysql_res = mysql_store_result(theApp.mysql);
			cols = mysql_num_fields(mysql_res);
			if (mysql_res)
			{
				if((mysql_row = mysql_fetch_row(mysql_res)))
				{
					if(strcmp(mysql_row[4],Msg.answer) == 0)
					{
						strcpy(Msg.password,mysql_row[2]) ;
						Msg.flag = 1 ;
					}
					else
					{
						Msg.flag = 0 ;
					}
				}
				else
				{
					Msg.flag = 0 ;
				}
				mysql_free_result(mysql_res);
			}
			break;
		}
	case 10:   //��Դ��̬
		{
			theApp.head.type = 2 ;
			strcpy(theApp.file_time,Msg.message);
			strcpy(theApp.file_nicname,Msg.nicname);
			strcpy(theApp.file_state,Msg.question);
			SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
			break;
		}

	}

	sendmsg(Msg);
	CSocket::OnReceive(nErrorCode);
}

void CTcpClient::OnClose(int nErrorCode)
{
	if(n_LoginErr == 1)
	{
		NULL ;
	}
	else
	{
		theApp.m_clientMessage.erase(m_strID);
		theApp.head.type = 1;
		SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
		CHead Msg;
		Msg.type = 0;
		strcpy(Msg.id,(CStringA)m_strID);
		strcpy_s(Msg.IP,(CStringA)m_strIp);
		sendmsg(Msg);	
	}	

	CSocket::OnClose(nErrorCode);
}

void CTcpClient::sendmsg(CHead headobj)
{
	POSITION ps = m_pList->GetHeadPosition();  //ȡ�ã������û��Ķ���
	switch(headobj.type)
	{
	case 0://���û��˳�
	case 1://���û�����
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->n_LoginErr == 1)
			{
				NULL ;
			}
			else
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}		
		}
		break;
	case 2://�������û���֪�������û�������
	case 3://������Ϣ		
		if(strcmp(headobj.toID,(CStringA)"10000") == 0) //Ⱥ��
		{  
			while(ps!=NULL)
			{
				CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
				if(pTemp->m_strID == headobj.fromID)  //�Լ������͸��Լ�
				{
					NULL;
				}	
				else  //����Ϣ���͸����ߵ�������
				{
		            pTemp->Send(&headobj,sizeof(headobj));
				}
				
			}
		}
		else  //˽��
		{
			while(ps!=NULL)
			{
				CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
				if(pTemp->m_strID == headobj.toID)
				{
					pTemp->Send(&headobj,sizeof(headobj));
				}			
			}
		}
		break;
	case 4://��¼
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == headobj.fromID)
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}
				
		}
		break;
	case 5:   //ע��  
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strIDtemp == headobj.message)
			{
				pTemp->Send(&headobj,sizeof(headobj));
				pTemp->m_strIDtemp = "";
			}
		}
		break;
	case 6:  //�����ܱ�����
		
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strIDtemp == headobj.message)
			{
				pTemp->Send(&headobj,sizeof(headobj));
				pTemp->m_strIDtemp = "";
			}
		}
		break;
	case 7:  //��������
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strIDtemp == headobj.message)
			{
				pTemp->Send(&headobj,sizeof(headobj));
				pTemp->m_strIDtemp = "";
			}
		}
		break;
	case 8: //�ж��Ƿ�����ļ�
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == headobj.toID)
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}		
		}
	case 9: //�����Ƿ�Ը�����
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == headobj.fromID)
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}		
		}
	default:
		{
			break;
		}
		
	}
}



