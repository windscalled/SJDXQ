#pragma once

// CMessage 命令目标

class CMessage : public CObject
{
public:

	int m_nType ;             //消息类型
	DWORD m_dwFileSize ;      //文件大小
	CString m_strFileName ;   //文件名称
	CHead myHead ;            //存储的信息结构体
	void Serialize(CArchive &ar) ;   //重载Serialize函数 串行化发送消息

	//重载构造函数
	CMessage(int nType,CString strFileName,DWORD dwFileSize);
	CMessage(int nType);
	CMessage();
	virtual ~CMessage();
};


