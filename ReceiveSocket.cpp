// ReceiveSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "ReceiveSocket.h"


// CReceiveSocket

CReceiveSocket::CReceiveSocket()
{
}

CReceiveSocket::~CReceiveSocket()
{
}

int nSize = 0; //每次收到的大小
char buf[512] = {0};//接收缓存
DWORD dwCount = 0;  //总共接收大小


// CReceiveSocket 成员函数
void CReceiveSocket::OnReceive(int nErrorCode)
{
	// TODO: 接收文件
    memset(buf,0,sizeof(buf));
	nSize = Receive(buf, 512);  
	//将接收到的文件写到新建的文件中去
	if (this->file==NULL)
	{
		AfxMessageBox(_T("文件创建发生错误!"));
		return;
	}

	if(strcmp(buf,"cancle_send!")==0)  //半途中收到取消发送文件通知
	{
		theApp.recve->Invalidate();
		AfxMessageBox(_T("对方已取消发送文件！"));
		theApp.recve->progress1.SetPos(0);
		Close();
		theApp.serverSoc->Close();
		dwCount = 0;//复原
		this->file.Close();//关闭文件
	}
	else
	{
		this->file.Write(buf,nSize);
		dwCount+=nSize;
		theApp.recve->progress1.SetPos(dwCount) ;
		theApp.recve->SetDlgItemInt(IDC_RECV,dwCount/1024);
		if (dwCount == theApp.recve->m_dwFileSize)
		{
			theApp.recve->Invalidate();
			AfxMessageBox(_T("成功接收文件"));
			theApp.recve->progress1.SetPos(0);
			Close();
			theApp.serverSoc->Close();
			dwCount = 0;//复原
			this->file.Close();//关闭文件
			theApp.recve->m_openfile.EnableWindow(TRUE);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

BOOL CReceiveSocket::OpenFile( CString filePath )
{
	if (!this->file.Open(filePath,CFile::modeCreate|CFile::modeReadWrite))
	{
		AfxMessageBox(_T("创建文件错误!"));
		return -1;
	}
	return 1;
}
