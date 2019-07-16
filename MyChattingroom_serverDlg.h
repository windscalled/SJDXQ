
// MyChattingroom_serverDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "afxwin.h"


// CMyChattingroom_serverDlg 对话框
class CMyChattingroom_serverDlg : public CDialogEx
{
// 构造
public:
	CMyChattingroom_serverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYCHATTINGROOM_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl my_listcontrol;
	afx_msg void OnClose();
	int tcp_start ;             //tcp启动标识
	bool server_start;          //服务器启动标识
	CUdpSocket *m_udpsocket;//UDP
	CTcpSocket *m_tcpsocket;//TCP
	afx_msg void OnBnClickedstart();
	CButton button1ctr;
	CButton button2ctr;
	afx_msg void OnBnClickedexit();
	CListCtrl m_fileListcontrol;
};
