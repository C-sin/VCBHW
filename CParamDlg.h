#pragma once
#include "afxdialogex.h"


// CParamDlg 对话框

class CParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParamDlg)

public:
	CParamDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CParamDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nSizeMode;
	CComboBox m_comboSize;
	afx_msg void OnBnClickedRadio2();
	int m_nSizeValue;
	CSpinButtonCtrl m_spinAngle;
	CSliderCtrl m_sliderLength;
	CComboBox m_comboDamping;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnCbnSelchangeCombo1();
	CScrollBar m_scrollLength;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_nLengthValue;

	int m_finalSize;       // 最终摆球尺寸
	int m_finalLength;     // 最终摆线长度
	int m_finalAngle;      // 最终初始角度
	double m_finalDamping; // 最终阻尼系数
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnCbnSelchangeCombo2();
};
