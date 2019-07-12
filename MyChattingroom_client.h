
// MyChattingroom_client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Login.h"
#include "MyChattingroom_clientDlg.h"
#include "Head.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "Chat.h"
#include "Mymusic.h"
#include "ReceiveServer.h"
#include "Share_file.h"


//#define REQUEST          0x03       //请求发送文件
//#define ACCEPT           0x04       //同意发送文件
//#define REFUSE           0x05       //拒绝发送文件
//#define CANCEL           0x06       //取消文件的发送
// CMyChattingroom_clientApp:
// 有关此类的实现，请参阅 MyChattingroom_client.cpp
//
struct apple 
{
	char mymesssage[1024] ;
};

class CMyChattingroom_clientApp : public CWinApp
{
public:
	CMyChattingroom_clientApp();
	CHead head ;            //传输协议包
	CMyChattingroom_clientDlg *active ;    //主窗口界面
	CLogin * log ;               //登陆界面               
	CModify * mod ;              //修改密码界面
	CQuestion * ques ;           //问题界面
	CUdpSocket * myudp ;         //udp
	CTcpSocket * mytcp ;         //tcp
	CReceivefile * recve ;       //文件传输界面
	CMymusic * music ;           //音乐播放界面
	CFile_transfer * sendFile ;  //文件传输界面
	CShare_file * share_file ;   //资源共享界面
	CChatrecord *cchat ;    //聊天记录界面

    int search ;            //服务器搜寻标志
	int tag ;               //账号和密码匹配标志
	int modify_tag ;        //找密判断标识
	CLogin login ;          //登录菜单
	CString sereverip ;     //记录服务器ip
	CString mylocalip ;     //本地ip
	CString otherlocalip ;  //文件传输中他人ip
	CString m_userid ;      //账号
	CString m_username ;    //用户名
	CString m_question ;    //密保问题
	CString m_password ;    //密码
	CString m_rename ;      //重命名
	map<CString,CChat* > m_usermessage ;   //记录聊天对话框
	map<CString,CString> m_clientmessage ; //记录在线好友的id，ip
	map<CString,CString> m_client ;        //记录在线好友的id，name
	CString friendid ;      //当前聊天对象id
	CString friendname ;    //当前聊天对象昵称
	CReceiveServer*serverSoc ;   //文件传输接收端
	CString ziyuan ;        //资源名 
	int nTime ;             //接收文件窗口出现次数
	CString str_LoadPath ;  //路径
private:
	void GetLocalIp(CString &ip) ;  //获取本地IP地址

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMyChattingroom_clientApp theApp;