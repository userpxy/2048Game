
// GameView.cpp: CGameView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGameView 构造/析构

CGameView::CGameView() noexcept
{
	// TODO: 在此处添加构造代码

}

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetFocus();
	ResizeWindow();
}

void CGameView::ResizeWindow() {
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);
	//  Calculate the difference
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();
	//  Change the window size based on the size of the game board
	rcWindow.right = rcWindow.left +
		pDoc->size * 180 + nWidthDiff;
	rcWindow.bottom = rcWindow.top +
		pDoc->size * 190 + nHeightDiff;
	//  The MoveWindow function resizes the frame window
	GetParentFrame()->MoveWindow(&rcWindow);
}

// CGameView 绘图

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDoc->cells.size())
		return;

	CRect rect;
	GetClientRect(&rect);

	float size = (min(rect.Width(), rect.Height() - 10)) / pDoc->size;
	float x = 0, y = 0, offset = .05f * size;

	

	CFont font;
	CFont* font_st;

	CRect valRect;
	CBrush brush;
	brush.CreateSolidBrush(pDoc->bkColor);
	CString val;

	if (pDoc->start && pDoc->cells.size()) {
		if (!pDoc->GetFreeCellsCount()) {
			pDoc->start = FALSE;
			AfxMessageBox(IDS_LOSE);
			return;
		}

		font_st = pDC->SelectObject(&font);

		pDC->SetBkColor(pDoc->bkColor);
		pDC->SetTextColor(RGB(255, 255, 255));

		pDC->SelectObject(&font);

		float startY =  2 * offset;

		// draw background rect		
		CBrush b(RGB(188, 176, 162));
		CPen p;
		p.CreatePen(PS_SOLID, 0, RGB(188, 176, 162));
		CBrush* oldB = pDC->SelectObject(&b);
		CPen* oldP = pDC->SelectObject(&p);

		CRect bkRect(0, startY, size * pDoc->size, startY + size * pDoc->size);
		pDC->RoundRect(bkRect, CPoint(10, 10));

		pDC->SelectObject(oldB);
		pDC->SelectObject(oldP);
		//

		for (int i = 0; i < pDoc->size; i++) {
			y = startY;
			for (int j = 0; j < pDoc->size; j++) {
				valRect.left = x + offset;
				valRect.top = y + offset;
				valRect.right = x + size - offset;
				valRect.bottom = y + size - offset;

				int color = GetColorByNumber(pDoc->cells.top()[i][j]);

				CPen pen;
				pen.CreatePen(PS_SOLID, 0, color);
				CBrush brush(color);
				CBrush* oldBrush = pDC->SelectObject(&brush);
				CPen* oldPen = pDC->SelectObject(&pen);
				int oldBkMode = pDC->SetBkMode(TRANSPARENT);

				pDC->RoundRect(valRect, CPoint(10, 10));

				if (pDoc->cells.top()[i][j]) {
					int oldTextColor = 0;
					if (pDoc->cells.top()[i][j] < 8)
						oldTextColor = pDC->SetTextColor(RGB(126, 117, 100));

					val.Format(L"%d", pDoc->cells.top()[i][j]);
					pDC->DrawTextW(val, valRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					if (oldTextColor)
						pDC->SetTextColor(oldTextColor);
				}

				y += size;

				pDC->SetBkMode(oldBkMode);
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
			}
			x += size;
		}

		
		pDC->SelectObject(font_st);

	}
	// TODO: 在此处为本机数据添加绘制代码
}


// CGameView 打印

BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGameView 诊断

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView 消息处理程序

void CGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGameDoc* pDoc = GetDocument();

	if (!pDoc->start) return;

	int** cell = new int* [pDoc->size];
	int** oldCell = pDoc->cells.top();

	for (int i = 0; i < pDoc->size; i++) {
		cell[i] = new int[pDoc->size];
		for (int j = 0; j < pDoc->size; j++) {
			cell[i][j] = oldCell[i][j];
		}
	}

	pDoc->cells.push(cell);

	int numMoves = 0;
	if (nChar == 37) {
		for (int j = 0; j < pDoc->size; j++) {
			for (int i = 0; i < pDoc->size; i++) {
				for (int k = i + 1; k < pDoc->size; k++) {
					if (pDoc->cells.top()[k][j] != 0) {
						if (pDoc->cells.top()[i][j] == 0) {
							numMoves++;

							pDoc->cells.top()[i][j] = pDoc->cells.top()[k][j];
							pDoc->cells.top()[k][j] = 0;
						}
						else {
							if (pDoc->cells.top()[i][j] == pDoc->cells.top()[k][j]) {
								numMoves++;

								pDoc->cells.top()[i][j] += pDoc->cells.top()[k][j];
								pDoc->points += pDoc->cells.top()[i][j];
								pDoc->cells.top()[k][j] = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	else if (nChar == 38) {
		for (int j = 0; j < pDoc->size; j++) {
			for (int i = 0; i < pDoc->size; i++) {
				for (int k = i + 1; k < pDoc->size; k++) {
					if (pDoc->cells.top()[j][k] != 0) {
						if (pDoc->cells.top()[j][i] == 0) {
							numMoves++;

							pDoc->cells.top()[j][i] = pDoc->cells.top()[j][k];
							pDoc->cells.top()[j][k] = 0;
						}
						else {
							if (pDoc->cells.top()[j][i] == pDoc->cells.top()[j][k]) {
								numMoves++;

								pDoc->cells.top()[j][i] += pDoc->cells.top()[j][k];
								pDoc->points += pDoc->cells.top()[j][i];
								pDoc->cells.top()[j][k] = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	else if (nChar == 39) {
		for (int j = pDoc->size - 1; j >= 0; j--) {
			for (int i = pDoc->size - 1; i >= 0; i--) {
				for (int k = i - 1; k >= 0; k--) {
					if (pDoc->cells.top()[k][j] != 0) {
						if (pDoc->cells.top()[i][j] == 0) {
							numMoves++;

							pDoc->cells.top()[i][j] = pDoc->cells.top()[k][j];
							pDoc->cells.top()[k][j] = 0;
						}
						else {
							if (pDoc->cells.top()[i][j] == pDoc->cells.top()[k][j]) {
								numMoves++;

								pDoc->cells.top()[i][j] += pDoc->cells.top()[k][j];
								pDoc->points += pDoc->cells.top()[i][j];
								pDoc->cells.top()[k][j] = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	else if (nChar == 40) {
		for (int j = pDoc->size - 1; j >= 0; j--) {
			for (int i = pDoc->size - 1; i >= 0; i--) {
				for (int k = i - 1; k >= 0; k--) {
					if (pDoc->cells.top()[j][k] != 0) {
						if (pDoc->cells.top()[j][i] == 0) {
							numMoves++;

							pDoc->cells.top()[j][i] = pDoc->cells.top()[j][k];
							pDoc->cells.top()[j][k] = 0;
						}
						else {
							if (pDoc->cells.top()[j][i] == pDoc->cells.top()[j][k]) {
								numMoves++;

								pDoc->cells.top()[j][i] += pDoc->cells.top()[j][k];
								pDoc->points += pDoc->cells.top()[j][i];
								pDoc->cells.top()[j][k] = 0;
							}
							break;
						}
					}
				}
			}
		}
	}

	if (numMoves) pDoc->GenerateNewRandomCell();
	else pDoc->cells.pop();

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}