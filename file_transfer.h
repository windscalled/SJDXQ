#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// file_transfer �Ի���

class CFile_transfer : public CDialogEx
{
	DECLARE_DYNAMIC(CFile_transfer)

public:
	CFile_transfer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFile_transfer();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit filepath;
	afx_msg void OnBnClickedButton2();
	CButton cancelsend;
	BOOL bStopSend;//ȡ�������ļ�
	
	CFont font ;
	CImage m_image1;
	CBrush m_Br2;
	void sendFile();
	CProgressCtrl m_ctrlProgress;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton3();
};
