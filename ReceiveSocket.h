#pragma once

// CReceiveSocket ����Ŀ��

class CReceiveSocket : public CSocket
{
public:
	CReceiveSocket();
	virtual ~CReceiveSocket();
	virtual void OnReceive(int nErrorCode);
	virtual BOOL OpenFile(CString filePath);
protected:

	CFile file;//Ҫ������ļ�����
};


