#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReceivefile �Ի���

class CReceivefile : public CDialogEx
{
	DECLARE_DYNAMIC(CReceivefile)

public:
	CReceivefile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReceivefile();

	//BOOL SetSaveFileFolder(LPCTSTR strFolder);
// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progress1;
	//�ļ���С
	DWORD m_dwFileSize;
	//Ҫ������ļ�
	CString  saveFilePath;
	CString  saveFileFolder;
	CButton m_openfile;
	void CreateReceiveSoket() ;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CFont font ;
	CImage m_image1;
	CBrush m_Br2;
	afx_msg void OnBnClickedButton2();
};
