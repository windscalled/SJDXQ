#pragma once

// CMessage ����Ŀ��

class CMessage : public CObject
{
public:

	int m_nType ;             //��Ϣ����
	DWORD m_dwFileSize ;      //�ļ���С
	CString m_strFileName ;   //�ļ�����
	CHead myHead ;            //�洢����Ϣ�ṹ��
	void Serialize(CArchive &ar) ;   //����Serialize���� ���л�������Ϣ

	//���ع��캯��
	CMessage(int nType,CString strFileName,DWORD dwFileSize);
	CMessage(int nType);
	CMessage();
	virtual ~CMessage();
};


