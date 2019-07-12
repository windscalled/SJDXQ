#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReceivefile 对话框

class CReceivefile : public CDialogEx
{
	DECLARE_DYNAMIC(CReceivefile)

public:
	CReceivefile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReceivefile();

	//BOOL SetSaveFileFolder(LPCTSTR strFolder);
// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progress1;
	//文件大小
	DWORD m_dwFileSize;
	//要保存的文件
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
