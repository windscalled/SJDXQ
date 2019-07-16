
// MyChattingroom_clientDlg.h : 头文件
//


#pragma once
#include "afxcmn.h"
#include "Mymusic.h"
#include "Share_file.h"
#include <Windows.h>
#define MY_NOTIFYICON  (WM_USER + 200)
// CMyChattingroom_clientDlg 对话框
class CMyChattingroom_clientDlg : public CDialogEx
{
// 构造
public:
	CMyChattingroom_clientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYCHATTINGROOM_CLIENT_DIALOG };

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
	
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);
	CListCtrl mylistcontrol;
	afx_msg void OnClose();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQuit();
	CString m_caption;
	afx_msg void OnBnClickedButton1();
	CBrush m_Br2 ;
	CImage m_image1;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMymusic  music ;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedMusic();

	int m_nWidth,m_nHeight;
	int m_nDx,m_nDy;
	HWND statu;	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	//添加托盘图标函数
	BOOL TaskBarAddIcon(HWND hwnd,UINT uID,HICON hicon,CString strTip);
	//删除托盘图标函数
	BOOL TaskBarDeleteIcon(HWND hwnd,UINT uID);
	//自定义消息函数
	LRESULT OnMyIconNotify(WPARAM wparam,LPARAM lparam);
	afx_msg void OnDestroy();
};
