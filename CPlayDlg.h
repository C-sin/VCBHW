#pragma once
#include "afxdialogex.h"
#include "COCX1.h"


// CPlayDlg 对话框

class CPlayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayDlg)

public:
	CPlayDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPlayDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COCX1 m_wmp;
	virtual BOOL OnInitDialog();
};
