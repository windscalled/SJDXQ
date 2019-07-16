// TcpClient.cpp : 实现文件
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


// CTcpClient 成员函数

void CTcpClient::OnReceive(int nErrorCode)
{
	int n,tag = 0;
	CHead Msg;
	n=Receive(&Msg,sizeof(Msg));
	switch(Msg.type)
	{
	case 1:    //登录信息
		{
			m_strID = Msg.id;
			m_strIp = Msg.IP;
			theApp.m_clientMessage.insert(make_pair(Msg.id,Msg.IP));
			theApp.head.type = 1;
			SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
			break;
		}
	case 4:    //登录
		{
			//在mysql中匹配账号和密码

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

				if(temp != 1)   //密码出错
				{
					Msg.flag = 0 ;
				    n_LoginErr = 1 ;
				}
				else
				{
					//判断是否重复登录
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
	case 5:   //注册信息
		{
			//在mysql中添加账户个人资料
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
					temp = 1 ;//未找到相同的账号		
				}
				else
				{		
					NULL;//该账号已被注册	
				}

				if(temp == 1)
				{
					int result;
					// 没有返回结果集的操作
					theApp.my_mysql();
					char buff[500];
					sprintf(buff,"insert into information values('%s','%s','%s','%s','%s');",Msg.nicname,Msg.id,Msg.password,Msg.question,Msg.answer);

					result = mysql_query(theApp.mysql,buff);
					if (result != 0)
					{
						AfxMessageBox(_T("注册数据失败，请检查！"));
						AfxMessageBox((CString)mysql_error(theApp.mysql));
					}
					else
					{
						//AfxMessageBox(_T("用户注册成功!"));
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
	case 6:    //判断有无此ID
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
						strcpy(Msg.question,mysql_row[3]) ;   //将密保问题发送给客户端
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
	case 7:    //判断回答问题并找回密码
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
	case 10:   //资源动态
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
	POSITION ps = m_pList->GetHeadPosition();  //取得，所有用户的队列
	switch(headobj.type)
	{
	case 0://有用户退出
	case 1://有用户上线
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
	case 2://已在线用户告知刚上线用户我在线
	case 3://发送消息		
		if(strcmp(headobj.toID,(CStringA)"10000") == 0) //群聊
		{  
			while(ps!=NULL)
			{
				CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
				if(pTemp->m_strID == headobj.fromID)  //自己不发送给自己
				{
					NULL;
				}	
				else  //将信息发送给在线的所有人
				{
		            pTemp->Send(&headobj,sizeof(headobj));
				}
				
			}
		}
		else  //私聊
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
	case 4://登录
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == headobj.fromID)
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}
				
		}
		break;
	case 5:   //注册  
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
	case 6:  //发送密保问题
		
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
	case 7:  //发送密码
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
	case 8: //判断是否接收文件
		while(ps!=NULL)
		{
			CTcpClient* pTemp = (CTcpClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == headobj.toID)
			{
				pTemp->Send(&headobj,sizeof(headobj));
			}		
		}
	case 9: //返回是否愿意接收
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



