// Message.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Message.h"


// CMessage

CMessage::CMessage()
{
	m_nType = -1;
	m_strFileName = _T("");
	m_dwFileSize = 0;
}

CMessage::~CMessage()
{
}


// CMessage 成员函数

//只需发送一般的控制信息时使用
CMessage::CMessage(int nType)
{
	m_nType = nType;
	m_strFileName = _T("");
	m_dwFileSize = 0;
}


//需要发送文件名及大小时使用
CMessage::CMessage(int nType, CString strFileName, DWORD dwFileSize)
{
	m_nType = nType;
	m_strFileName = strFileName;
	m_dwFileSize = dwFileSize;
}


//实现串行化
void CMessage::Serialize(CArchive &ar)    
{
	if(ar.IsStoring())
	{
		ar<<m_nType;
		ar<<m_strFileName;
		ar<<m_dwFileSize;
	}
	else
	{
		ar>>m_nType;
		ar>>m_strFileName;
		ar>>m_dwFileSize;
	}
}




