#pragma once


struct CHead 
{
	char id[6];              //�����˺�
	char hobby[20] ;
	char nicname[512];       //�ǳ�
	char password[20];       //����
	char question[512] ;     //�ܱ�����
	char answer[512] ;       //�ܱ���

	int type;           //�������������Ϣ����
	int flag;           //�˺�����ƥ���ʶ
	char fromID[10];    //��ϢԴ�ͻ����û���
	char toID[10];      //��ϢĿ�Ŀͻ����û���
	char IP[32];
	char message[1024]; //��Ϣ����
	DWORD m_dwFileSize;//�ļ���С
};
