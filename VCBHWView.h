
// VCBHWView.h: CVCBHWView 类的接口
//

#pragma once
#include <deque>


class CVCBHWView : public CView
{
protected: // 仅从序列化创建
	CVCBHWView() noexcept;
	DECLARE_DYNCREATE(CVCBHWView)

// 特性
public:
	CVCBHWDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CVCBHWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSimSettings();
	afx_msg void OnSimPlayvideo();

public:
	// 单摆物理与状态变量
	bool   m_bIsRunning;   // 是否正在运动
	double m_angle;        // 当前角度 (弧度制)
	double m_omega;        // 角速度
	double m_length;       // 摆线长度 (像素)
	int    m_ballSize;     // 摆球尺寸 (半径)
	double m_damping;      // 阻尼系数

	double m_timeElapsed;  // 记录已经运行的时间 (秒)，用于10秒停止判断

	//残影参量
	struct TrailState
	{
		double theta; // 角度(弧度)
		double omega; // 角速度
		bool stopped; //这个残影点是否已到达并冻结
	};

	std::deque<TrailState> m_trail;  
	double m_finalAngle = 0.0;        // 最终角度（成员变量）
	const int m_trailLength = 9;     // 残影长度
	int m_trailSkipCounter = 0; // 已经跳过的帧数
	int m_trailSkip = 3;        // 每3帧记录一次（2~5都可调）
	bool m_bTrailSettling;     // 是否处于“残影收敛”阶段
	bool m_bFinishedNotified;  // 是否已经弹过结束提示（防止弹两次）

	// 悬挂点坐标 (画图时的原点)
	CPoint m_ptOrigin;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // VCBHWView.cpp 中的调试版本
inline CVCBHWDoc* CVCBHWView::GetDocument() const
   { return reinterpret_cast<CVCBHWDoc*>(m_pDocument); }
#endif

