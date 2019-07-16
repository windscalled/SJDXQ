#pragma once


struct CHead 
{
	char id[6];              //个人账号
	char hobby[20] ;
	char nicname[512];       //昵称
	char password[20];       //密码
	char question[512] ;     //密保问题
	char answer[512] ;       //密保答案

	int type;           //传入服务器的信息类型
	int flag;           //账号密码匹配标识
	char fromID[10];    //信息源客户端用户名
	char toID[10];      //信息目的客户端用户名
	char IP[32];
	char message[1024]; //信息内容
	DWORD m_dwFileSize;//文件大小
};
