// ReceiveSocket.cpp : ʵ���ļ�
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

int nSize = 0; //ÿ���յ��Ĵ�С
char buf[512] = {0};//���ջ���
DWORD dwCount = 0;  //�ܹ����մ�С


// CReceiveSocket ��Ա����
void CReceiveSocket::OnReceive(int nErrorCode)
{
	// TODO: �����ļ�
    memset(buf,0,sizeof(buf));
	nSize = Receive(buf, 512);  
	//�����յ����ļ�д���½����ļ���ȥ
	if (this->file==NULL)
	{
		AfxMessageBox(_T("�ļ�������������!"));
		return;
	}

	if(strcmp(buf,"cancle_send!")==0)  //��;���յ�ȡ�������ļ�֪ͨ
	{
		theApp.recve->Invalidate();
		AfxMessageBox(_T("�Է���ȡ�������ļ���"));
		theApp.recve->progress1.SetPos(0);
		Close();
		theApp.serverSoc->Close();
		dwCount = 0;//��ԭ
		this->file.Close();//�ر��ļ�
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
			AfxMessageBox(_T("�ɹ������ļ�"));
			theApp.recve->progress1.SetPos(0);
			Close();
			theApp.serverSoc->Close();
			dwCount = 0;//��ԭ
			this->file.Close();//�ر��ļ�
			theApp.recve->m_openfile.EnableWindow(TRUE);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

BOOL CReceiveSocket::OpenFile( CString filePath )
{
	if (!this->file.Open(filePath,CFile::modeCreate|CFile::modeReadWrite))
	{
		AfxMessageBox(_T("�����ļ�����!"));
		return -1;
	}
	return 1;
}
