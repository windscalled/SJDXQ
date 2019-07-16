
// MyChattingroom_serverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "afxwin.h"


// CMyChattingroom_serverDlg �Ի���
class CMyChattingroom_serverDlg : public CDialogEx
{
// ����
public:
	CMyChattingroom_serverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYCHATTINGROOM_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl my_listcontrol;
	afx_msg void OnClose();
	int tcp_start ;             //tcp������ʶ
	bool server_start;          //������������ʶ
	CUdpSocket *m_udpsocket;//UDP
	CTcpSocket *m_tcpsocket;//TCP
	afx_msg void OnBnClickedstart();
	CButton button1ctr;
	CButton button2ctr;
	afx_msg void OnBnClickedexit();
	CListCtrl m_fileListcontrol;
};
