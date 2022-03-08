
// GameDoc.cpp: CGameDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()


// CGameDoc 构造/析构

CGameDoc::CGameDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	points = 0;
	size = 4;
	bkColor = RGB(214, 205, 196);
}

CGameDoc::~CGameDoc()
{
	if (cells.size()) {
		while (cells.size()) {
			int** cell = cells.top();
			cells.pop();

			for (int i = 0; i < size; i++)
				delete cell[i];

			delete[] cell;
		}
	}
}

BOOL CGameDoc::OnNewDocument()
{
	srand(time(NULL));

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	int** cell = new int* [size];
	for (int i = 0; i < size; i++) {
		cell[i] = new int[size];
		for (int j = 0; j < size; j++)
			cell[i][j] = 0;
	}

	int filledCount = rand() % (size * 2 - 1) + 2;
	std::set<std::pair<int, int>> uniqElem;
	std::set<std::pair<int, int>>::iterator it;

	while (filledCount >= 0) {
		int value = (rand() % 2 + 1) * 2;
		std::pair<int, int> coord;

		do {
			coord.first = rand() % size;
			coord.second = rand() % size;

			it = uniqElem.find(coord);
		} while (it != uniqElem.end());

		cell[coord.first][coord.second] = value;

		uniqElem.insert(coord);

		filledCount--;
	}

	start = true;
	cells.push(cell);

	return TRUE;
}




// CGameDoc 序列化

void CGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CGameDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGameDoc 诊断

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameDoc 命令
void CGameDoc::GenerateNewRandomCell()
{
	if (cells.top() == nullptr) return;

	std::vector<std::pair<int, int>> freeCells;

	// get free cells
	GetFreeCells(freeCells);

	// fill random cell
	pair<int, int> randCoord = freeCells[rand() % freeCells.size()];
	cells.top()[randCoord.first][randCoord.second] = (rand() % 2 + 1) * 2;
}


// Function that fill std::vector elements with std::pair that contain coord of free cells
void CGameDoc::GetFreeCells(vector<pair<int, int>>& in)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (!cells.top()[i][j]) in.push_back(pair<int, int>(i, j));
}


int CGameDoc::GetFreeCellsCount()
{
	vector<pair<int, int>> freeCells;
	GetFreeCells(freeCells);

	return freeCells.size();
}