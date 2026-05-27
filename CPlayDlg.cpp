// CPlayDlg.cpp: 实现文件
//

#include "pch.h"
#include "VCBHW.h"
#include "afxdialogex.h"
#include "CPlayDlg.h"
#include <shellapi.h>


// CPlayDlg 对话框

IMPLEMENT_DYNAMIC(CPlayDlg, CDialogEx)

CPlayDlg::CPlayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PLAY, pParent)
{

}

CPlayDlg::~CPlayDlg()
{
}

void CPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX1, m_wmp);
}


BEGIN_MESSAGE_MAP(CPlayDlg, CDialogEx)
END_MESSAGE_MAP()


// CPlayDlg 消息处理程序

BOOL CPlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShellExecute(nullptr, _T("open"), _T("Narration.avi"), nullptr, nullptr, SW_SHOWNORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
