
// MyChattingroom_client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Login.h"
#include "MyChattingroom_clientDlg.h"
#include "Head.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "Chat.h"
#include "Mymusic.h"
#include "ReceiveServer.h"
#include "Share_file.h"


//#define REQUEST          0x03       //�������ļ�
//#define ACCEPT           0x04       //ͬ�ⷢ���ļ�
//#define REFUSE           0x05       //�ܾ������ļ�
//#define CANCEL           0x06       //ȡ���ļ��ķ���
// CMyChattingroom_clientApp:
// �йش����ʵ�֣������ MyChattingroom_client.cpp
//
struct apple 
{
	char mymesssage[1024] ;
};

class CMyChattingroom_clientApp : public CWinApp
{
public:
	CMyChattingroom_clientApp();
	CHead head ;            //����Э���
	CMyChattingroom_clientDlg *active ;    //�����ڽ���
	CLogin * log ;               //��½����               
	CModify * mod ;              //�޸��������
	CQuestion * ques ;           //�������
	CUdpSocket * myudp ;         //udp
	CTcpSocket * mytcp ;         //tcp
	CReceivefile * recve ;       //�ļ��������
	CMymusic * music ;           //���ֲ��Ž���
	CFile_transfer * sendFile ;  //�ļ��������
	CShare_file * share_file ;   //��Դ�������
	CChatrecord *cchat ;    //�����¼����

    int search ;            //��������Ѱ��־
	int tag ;               //�˺ź�����ƥ���־
	int modify_tag ;        //�����жϱ�ʶ
	CLogin login ;          //��¼�˵�
	CString sereverip ;     //��¼������ip
	CString mylocalip ;     //����ip
	CString otherlocalip ;  //�ļ�����������ip
	CString m_userid ;      //�˺�
	CString m_username ;    //�û���
	CString m_question ;    //�ܱ�����
	CString m_password ;    //����
	CString m_rename ;      //������
	map<CString,CChat* > m_usermessage ;   //��¼����Ի���
	map<CString,CString> m_clientmessage ; //��¼���ߺ��ѵ�id��ip
	map<CString,CString> m_client ;        //��¼���ߺ��ѵ�id��name
	CString friendid ;      //��ǰ�������id
	CString friendname ;    //��ǰ��������ǳ�
	CReceiveServer*serverSoc ;   //�ļ�������ն�
	CString ziyuan ;        //��Դ�� 
	int nTime ;             //�����ļ����ڳ��ִ���
	CString str_LoadPath ;  //·��
private:
	void GetLocalIp(CString &ip) ;  //��ȡ����IP��ַ

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyChattingroom_clientApp theApp;