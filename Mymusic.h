#pragma once
#include "afxcmn.h"


// CMymusic 对话框

class CMymusic : public CDialogEx
{
	DECLARE_DYNAMIC(CMymusic)

public:
	CMymusic(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMymusic();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int myint;
	CSliderCtrl myslider;  //滑动条
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CImage m_image1;
	CBrush m_Br2;
	CSliderCtrl m_SliderCtl;
	int m_nSlider;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CListCtrl m_music_listcontrol;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
