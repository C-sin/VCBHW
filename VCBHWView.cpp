
// VCBHWView.cpp: CVCBHWView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "VCBHW.h"
#endif

#include "VCBHWDoc.h"
#include "VCBHWView.h"

#include "CParamDlg.h"
#include "CPlayDlg.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVCBHWView

IMPLEMENT_DYNCREATE(CVCBHWView, CView)

BEGIN_MESSAGE_MAP(CVCBHWView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVCBHWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SIM_SETTINGS, &CVCBHWView::OnSimSettings)
	ON_COMMAND(ID_SIM_PLAYVIDEO, &CVCBHWView::OnSimPlayvideo)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVCBHWView 构造/析构

CVCBHWView::CVCBHWView() noexcept
{
	// TODO: 在此处添加构造代码
	m_bIsRunning = false;
	m_angle = 0.0;
	m_omega = 0.0;
	m_length = 100.0;
	m_ballSize = 20;
	m_damping = 0.0;
	m_timeElapsed = 0.0;
	m_ptOrigin = CPoint(400, 100); // 假设悬挂点在窗口靠上的中间位置
	m_bTrailSettling = false;
	m_bFinishedNotified = false;
}

CVCBHWView::~CVCBHWView()
{
}

BOOL CVCBHWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CVCBHWView 绘图

void CVCBHWView::OnDraw(CDC* pDC)
{
	CVCBHWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	// 取窗口尺寸
	CRect rect;
	GetClientRect(&rect);

	// ===== 双缓冲：画到内存 =====
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBitmap);

	// 背景清屏（你也可以换成渐变背景）
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	// 后面统一用这个 DC 画
	CDC* pDrawDC = &memDC;

	// 原点位置（悬挂点）
	m_ptOrigin.x = rect.Width() / 2;
	m_ptOrigin.y = 100;

	if (!m_bIsRunning && m_angle == 0.0 && m_timeElapsed == 0.0)
	{
		CFont warningFont;
		warningFont.CreatePointFont(120, _T("宋体"));
		CFont* pOldFont = pDrawDC->SelectObject(&warningFont);

		pDrawDC->SetTextColor(RGB(255, 0, 0));
		pDrawDC->SetBkMode(TRANSPARENT);
		pDrawDC->DrawText(_T("请先通过菜单设置参数并开始模拟！"),
			CRect(10, 10, rect.Width() - 10, 80),
			DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		pDrawDC->SelectObject(pOldFont);
	}
	else
	{
		// 计算摆球位置（你这里 *4 会放大摆幅，没问题但注意别出界）
		int ballX = m_ptOrigin.x + (int)(m_length * 4 * sin(m_angle));
		int ballY = m_ptOrigin.y + (int)(m_length * 4 * cos(m_angle));

		// 画天花板
		CPen myPen(PS_SOLID, 3, RGB(0, 0, 0));
		CPen* pOldPen = pDrawDC->SelectObject(&myPen);
		pDrawDC->MoveTo(m_ptOrigin.x - 200, m_ptOrigin.y);
		pDrawDC->LineTo(m_ptOrigin.x + 200, m_ptOrigin.y);

		// 画摆线
		pDrawDC->MoveTo(m_ptOrigin);
		pDrawDC->LineTo(ballX, ballY);
		pDrawDC->SelectObject(pOldPen);

		// 画残影
		for (size_t i = 0; i < m_trail.size(); ++i)
		{
			COLORREF color = RGB(150 + 70 * i / m_trailLength, 190, 255);
			CBrush trailBrush(color);
			CBrush* pOld = pDrawDC->SelectObject(&trailBrush);

			int r = m_ballSize - (m_ballSize * (m_trailLength - (int)i) / m_trailLength / 2);
			if (r < 1) r = 1;

			double a = m_trail[i].theta;
			int x = m_ptOrigin.x + (int)(m_length * 4 * sin(a));
			int y = m_ptOrigin.y + (int)(m_length * 4 * cos(a));
			pDrawDC->Ellipse(x - r, y - r, x + r, y + r);

			pDrawDC->SelectObject(pOld);
		}

		// 影子（建议不要写死 800，用窗口底部更稳）
		int shadowX = ballX;
		int shadowY = rect.bottom - 80; // 距离底部 80 像素，你可调
		int shadowWidth = m_ballSize * 2;
		int shadowHeight = max(2, m_ballSize / 2);

		CBrush shadowBrush(RGB(100, 100, 100));
		CBrush* pOldBrush = pDrawDC->SelectObject(&shadowBrush);
		pDrawDC->Ellipse(shadowX - shadowWidth, shadowY - shadowHeight,
			shadowX + shadowWidth, shadowY + shadowHeight);
		pDrawDC->SelectObject(pOldBrush);

		// 画摆球
		CBrush ballBrush(RGB(0, 120, 215));
		CBrush* pOldBrush2 = pDrawDC->SelectObject(&ballBrush);
		pDrawDC->Ellipse(ballX - m_ballSize, ballY - m_ballSize,
			ballX + m_ballSize, ballY + m_ballSize);
		pDrawDC->SelectObject(pOldBrush2);
	}

	// 最后一次性拷贝到屏幕 DC
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 清理 GDI 对象
	memDC.SelectObject(pOldBmp);
}


// CVCBHWView 打印


void CVCBHWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVCBHWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CVCBHWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CVCBHWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CVCBHWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVCBHWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVCBHWView 诊断

#ifdef _DEBUG
void CVCBHWView::AssertValid() const
{
	CView::AssertValid();
}

void CVCBHWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVCBHWDoc* CVCBHWView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVCBHWDoc)));
	return (CVCBHWDoc*)m_pDocument;
}
#endif //_DEBUG


// CVCBHWView 消息处理程序

void CVCBHWView::OnSimSettings()
{
	// TODO: 在此添加命令处理程序代码
	CParamDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strTest;
		strTest.Format(_T("收到参数：尺寸%d, 长度%d, 角度%d, 阻尼%.2f"),
			dlg.m_finalSize, dlg.m_finalLength, dlg.m_finalAngle, dlg.m_finalDamping);
		AfxMessageBox(strTest);

		// TODO: 下一步就是在这里启动定时器，开始物理模拟！

		// 1. 接收参数并转换
		m_ballSize = 10 + dlg.m_finalSize ;
		m_length = dlg.m_finalLength;
		m_damping = dlg.m_finalDamping;

		// 物理计算通常用弧度，所以把角度转成弧度：弧度 = 角度 * PI / 180
		// 注意：向右摆开通常角度为正，所以这里可以直接转换
		m_angle = dlg.m_finalAngle * 3.1415926535 / 180.0;

		// 2. 初始化运动状态
		m_omega = 0.0;         // 初始释放时速度为0
		m_timeElapsed = 0.0;   // 计时归零
		m_bIsRunning = true;   // 标记正在运动
		
		//3.初始化残影
		m_trail.clear();
		m_trailSkipCounter = 0;
		m_bFinishedNotified = false;

		// 4. 启动定时器！
		// 参数1是定时器ID，参数2是间隔时间(20毫秒，相当于50帧每秒)，参数3固定为NULL
		SetTimer(1, 20, NULL);
	}
}

void CVCBHWView::OnSimPlayvideo()
{
	// TODO: 在此添加命令处理程序代码
	CPlayDlg dlg;
	dlg.DoModal();
}

void CVCBHWView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	m_ptOrigin.x = rect.Width() / 2;
	m_ptOrigin.y = 100;

	if (nIDEvent == 1 && m_bIsRunning)
	{
		double dt = 0.02;
		m_timeElapsed += dt;

		//物理公式：角加速度 = - (g / L) * sin(角度) - 阻尼 * 角速度
		double g = 9.8; // 重力加速度
		double effective_L = m_length / 80.0;// 为了让动画在屏幕上看起来自然，我们把像素长度缩小一点作为物理长度
		double alpha = -(g / effective_L) * sin(m_angle) - m_damping * m_omega;

		// 更新角速度和角度
		m_omega += alpha * dt;
		m_angle += m_omega * dt;

		if (m_timeElapsed >= 4.0 || (fabs(m_angle) < 0.005 && fabs(m_omega) < 0.05)) // 10秒后自动停止
		{
			m_bIsRunning = false;
			//KillTimer(1); // 停止定时器
			m_bTrailSettling = true;   // 进入残影收敛阶段
			m_bFinishedNotified = false; // 允许后面弹一次

			m_finalAngle = m_angle;
		}
	}

		//绘制残影阶段
	int ballX = m_ptOrigin.x + (int)(m_length * 4 * sin(m_angle));
	int ballY = m_ptOrigin.y + (int)(m_length * 4 * cos(m_angle));
	CPoint target(ballX, ballY);

	if (nIDEvent == 1) {
		if (m_bIsRunning) {//正常运动绘制残影
			// 每隔 m_trailSkip 帧记录一次残影
			m_trailSkipCounter++;
			if (m_trailSkipCounter >= m_trailSkip)
			{
				m_trailSkipCounter = 0;

				m_trail.push_back({ m_angle, m_omega, false });
				if (m_trail.size() > m_trailLength) m_trail.pop_front();
			}
		}
		else if (m_bTrailSettling) {//残影收敛阶段
			double dt = 0.02;
			double g = 9.8;
			double effective_L = m_length / 80.0;

			// 用更强一点的阻尼让拖尾更快“停住”
			double damp = max(m_damping, 0.05);

			const double snapAngle = 0.02; // 角度阈值（弧度）

			bool anyMoving = false;

			for (auto& t : m_trail)
			{
				if (t.stopped) continue; // 已冻结的不再更新

				double e = t.theta - m_finalAngle;

				// 到达：钳位 + 冻结
				if (fabs(e) < snapAngle)
				{
					t.theta = m_finalAngle;
					t.omega = 0.0;
					t.stopped = true;
					continue;
				}

				// 没到达：按同一套单摆方程更新（同速度模型）
				double alpha = -(g / effective_L) * sin(e) - damp * t.omega;
				t.omega += alpha * dt;
				t.theta += t.omega * dt;

				anyMoving = true;
			}

			if (!anyMoving)
			{
				for (auto& t : m_trail)
				{
					t.theta = m_finalAngle;
					t.omega = 0.0;
					t.stopped = true;
				}

				m_bTrailSettling = false;
				SetTimer(2, 500, NULL);  // 设置新定时器用于弹窗
				KillTimer(1); // 这时才真正停
			}
		}
	}

	else if (nIDEvent == 2) {
		KillTimer(2); // 停止这个定时器

		// 收敛完成后再弹框（不会阻塞动画）
		if (!m_bFinishedNotified)
		{
			m_bFinishedNotified = true;

			int endX = (int)(m_length * sin(m_angle));
			int endY = (int)(-m_length * cos(m_angle));
			CString strMsg;
			strMsg.Format(_T("模拟结束！\n运动时长：%.2f 秒\n摆球位置: (%d, %d)"),
				m_timeElapsed, endX, endY);
			AfxMessageBox(strMsg);
		}
	}

		// 计算完后，通知窗口重新画图
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}

BOOL CVCBHWView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}
