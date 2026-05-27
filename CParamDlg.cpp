// CParamDlg.cpp: 实现文件
//

#include "pch.h"
#include "VCBHW.h"
#include "afxdialogex.h"
#include "CParamDlg.h"


// CParamDlg 对话框

IMPLEMENT_DYNAMIC(CParamDlg, CDialogEx)

CParamDlg::CParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTINGS, pParent)
	, m_nSizeMode(0)
	, m_nSizeValue(0)
	, m_nLengthValue(0)
{

}

CParamDlg::~CParamDlg()
{
}

void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nSizeMode);
	DDX_Control(pDX, IDC_COMBO1, m_comboSize);
	DDX_Text(pDX, IDC_EDIT2, m_nSizeValue);
	DDX_Control(pDX, IDC_SPIN1, m_spinAngle);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderLength);
	DDX_Control(pDX, IDC_COMBO2, m_comboDamping);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollLength);
	DDX_Text(pDX, IDC_EDIT4, m_nLengthValue);
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CParamDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CParamDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CParamDlg::OnBnClickedRadio3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CParamDlg::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_START, &CParamDlg::OnBnClickedBtnStart)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CParamDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CParamDlg 消息处理程序

void CParamDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面最新状态
	m_nSizeValue = 20; // 设置编辑框的值
	m_comboSize.SetCurSel(-1); // 取消组合框的选中状态（因为现在选的是单选按钮）
	UpdateData(FALSE); // 把值刷新到界面上
}

BOOL CParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
		// 1. 设置角度 Spin 范围 (10 到 90)
	m_spinAngle.SetRange(10, 90);
	m_spinAngle.SetPos(30); // 默认 30 度

	// 2. 设置滑块范围 (例如长度 50 到 200)
	m_sliderLength.SetRange(50, 200);
	m_sliderLength.SetPos(100); // 默认 100

	// 3. 摆球尺寸组合框添加选项（假设单选有3个值，这里补2个凑够5个）
	m_comboSize.AddString(_T("尺寸选项 4 (40)"));
	m_comboSize.AddString(_T("尺寸选项 5 (50)"));

	// 4. 阻尼系数添加选项
	m_comboDamping.AddString(_T("无阻尼 (0.0)"));
	m_comboDamping.AddString(_T("弱阻尼 (0.02)"));
	m_comboDamping.AddString(_T("强阻尼 (0.05)"));
	m_comboDamping.AddString(_T("超强阻尼 (0.15)"));

	// 5. 设置单选按钮默认选第一个
	m_nSizeMode = 0;
	m_nSizeValue = 10; // 假设第一个尺寸值是 10
	m_nLengthValue = 100;
	UpdateData(FALSE); // 把变量的值更新到界面上

	// 设置水平滚动条的范围和初始位置 (和滑块保持一致)
	m_scrollLength.SetScrollRange(50, 200);
	m_scrollLength.SetScrollPos(100);
	// 设置长度编辑框的默认值（和滑块的默认值 100 保持一致）

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CParamDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面最新状态
	m_nSizeValue = 10; // 设置编辑框的值
	m_comboSize.SetCurSel(-1); // 取消组合框的选中状态（因为现在选的是单选按钮）
	UpdateData(FALSE); // 把值刷新到界面上
}

void CParamDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 获取界面最新状态
	m_nSizeValue = 30; // 设置编辑框的值
	m_comboSize.SetCurSel(-1); // 取消组合框的选中状态（因为现在选的是单选按钮）
	UpdateData(FALSE); // 把值刷新到界面上
}

void CParamDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_comboSize.GetCurSel(); // 获取选了第几项
	if (sel == 0) m_nSizeValue = 40;   // 组合框第一项是40
	if (sel == 1) m_nSizeValue = 50;   // 组合框第二项是50

	m_nSizeMode = -1; // 取消单选按钮的选中状态
	UpdateData(FALSE); // 刷新界面
}

void CParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = 100;

	// 1. 如果拖动的是“滑块”
	if (pScrollBar->GetSafeHwnd() == m_sliderLength.GetSafeHwnd())
	{
		pos = m_sliderLength.GetPos(); // 获取滑块当前位置
		m_scrollLength.SetScrollPos(pos);  // 让滚动条跟着动
	}
	// 2. 如果拖动的是“水平滚动条”
	else if (pScrollBar->GetSafeHwnd() == m_scrollLength.GetSafeHwnd())
	{
		pos = m_scrollLength.GetScrollPos(); // 获取滚动条当前位置
		// 处理滚动条的各种点击动作 (左移、右移、拖动等)
		switch (nSBCode)
		{
		case SB_LINELEFT: pos -= 1; break;
		case SB_LINERIGHT: pos += 1; break;
		case SB_PAGELEFT: pos -= 10; break;
		case SB_PAGERIGHT: pos += 10; break;
		case SB_THUMBTRACK: pos = nPos; break; // 拖动滑块本身
		}
		// 限制范围在 50 到 200 之间
		if (pos < 50) pos = 50;
		if (pos > 200) pos = 200;

		m_scrollLength.SetScrollPos(pos); // 更新滚动条位置
		m_sliderLength.SetPos(pos);       // 让滑竿跟着动
	}

	UpdateData(TRUE); // 获取界面最新状态
	m_nLengthValue = pos; // 设置编辑框的值
	UpdateData(FALSE); // 把值刷新到界面上

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CParamDlg::OnBnClickedBtnStart()
{
	UpdateData(TRUE); // 获取最新的单选/编辑框状态

	// 1. 提取尺寸（直接拿我们做联动的那个编辑框的值）
	m_finalSize = m_nSizeValue;

	// 2. 提取长度（从滑块获取）
	m_finalLength = m_sliderLength.GetPos();

	// 3. 提取角度（从Spin控件获取）
	m_finalAngle = m_spinAngle.GetPos32();

	// 关闭对话框，并返回 IDOK 标志给主窗口
	CDialogEx::OnOK();
}

void CParamDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); // 读入其它编辑框/单选框等

	int sel = m_comboDamping.GetCurSel();

	// 这里按你的组合框条目顺序映射阻尼值
	switch (sel)
	{
	case 3: m_finalDamping = 0.00; break; // 无阻尼
	case 2: m_finalDamping = 0.02; break; // 弱阻尼
	case 1: m_finalDamping = 0.05; break; // 强阻尼
	case 0: m_finalDamping = 0.15; break; // 超强阻尼
	default: m_finalDamping = 0.00; break;
	}
}
