
// GameView.h: CGameView 类的接口
//

#pragma once
#include "GameDoc.h"

class CGameView : public CView
{
protected: // 仅从序列化创建
	CGameView() noexcept;
	DECLARE_DYNCREATE(CGameView)

// 特性
public:
	CGameDoc* GetDocument() const;

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
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	void ResizeWindow();
	inline int GetColorByNumber(int number)
	{
		switch (number) {
		case 0:
			return RGB(214, 205, 196);
		case 2:
			return RGB(238, 228, 218);
		case 4:
			return RGB(236, 224, 200);
		case 8:
			return RGB(242, 177, 121);
		case 16:
			return RGB(245, 149, 99);
		case 32:
			return RGB(243, 125, 95);
		case 64:
			return RGB(246, 93, 59);
		case 128:
		case 256:
		case 512:
		case 1024:
		case 2048:
			return RGB(237, 204, 99);
		default:
			return RGB(255, 30, 31);
		}
};
};

#ifndef _DEBUG  // GameView.cpp 中的调试版本
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

