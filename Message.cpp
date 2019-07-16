// Message.cpp : ʵ���ļ�
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


// CMessage ��Ա����

//ֻ�跢��һ��Ŀ�����Ϣʱʹ��
CMessage::CMessage(int nType)
{
	m_nType = nType;
	m_strFileName = _T("");
	m_dwFileSize = 0;
}


//��Ҫ�����ļ�������Сʱʹ��
CMessage::CMessage(int nType, CString strFileName, DWORD dwFileSize)
{
	m_nType = nType;
	m_strFileName = strFileName;
	m_dwFileSize = dwFileSize;
}


//ʵ�ִ��л�
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




