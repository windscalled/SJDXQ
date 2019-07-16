
// MyChattingroom_clientDlg.h : ͷ�ļ�
//


#pragma once
#include "afxcmn.h"
#include "Mymusic.h"
#include "Share_file.h"
#include <Windows.h>
#define MY_NOTIFYICON  (WM_USER + 200)
// CMyChattingroom_clientDlg �Ի���
class CMyChattingroom_clientDlg : public CDialogEx
{
// ����
public:
	CMyChattingroom_clientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYCHATTINGROOM_CLIENT_DIALOG };

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
	//�������ͼ�꺯��
	BOOL TaskBarAddIcon(HWND hwnd,UINT uID,HICON hicon,CString strTip);
	//ɾ������ͼ�꺯��
	BOOL TaskBarDeleteIcon(HWND hwnd,UINT uID);
	//�Զ�����Ϣ����
	LRESULT OnMyIconNotify(WPARAM wparam,LPARAM lparam);
	afx_msg void OnDestroy();
};
