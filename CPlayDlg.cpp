// CPlayDlg.cpp: 实现文件
//

#include "pch.h"
#include "VCBHW.h"
#include "afxdialogex.h"
#include "CPlayDlg.h"
#include <shellapi.h>
#include <Windows.h>  


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

    CString exePath;
    GetModuleFileName(nullptr, exePath.GetBuffer(MAX_PATH), MAX_PATH);
    exePath.ReleaseBuffer();

    int pos = exePath.ReverseFind(_T('\\'));
    CString exeDir = (pos >= 0) ? exePath.Left(pos + 1) : _T("");

    CString videoPath = exeDir + _T("Narration.avi");

    // 可选：文件不存在就提示一次（不想提示也可以直接 return）
    DWORD attr = GetFileAttributes(videoPath);
    if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        AfxMessageBox(_T("找不到视频文件：\n") + videoPath);
        return TRUE;
    }

    // 用 WMP ActiveX 控件播放
    m_wmp.put_uiMode(_T("full"));
    m_wmp.put_stretchToFit(TRUE);
    m_wmp.put_URL(videoPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
