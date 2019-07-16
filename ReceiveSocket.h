#pragma once

// CReceiveSocket 命令目标

class CReceiveSocket : public CSocket
{
public:
	CReceiveSocket();
	virtual ~CReceiveSocket();
	virtual void OnReceive(int nErrorCode);
	virtual BOOL OpenFile(CString filePath);
protected:

	CFile file;//要保存的文件变量
};


