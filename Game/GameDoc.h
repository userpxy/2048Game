
// GameDoc.h: CGameDoc 类的接口
//



#pragma once
#include <stdlib.h>
#include <time.h>
#include <propkey.h>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

using std::stack;
using std::vector;
using std::pair;


class CGameDoc : public CDocument
{
protected: // 仅从序列化创建
	CGameDoc() noexcept;
	DECLARE_DYNCREATE(CGameDoc)

// 特性
public:
	int points;
	int size;
	stack<int**> cells;

	COLORREF bkColor;
	bool start;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	void GenerateNewRandomCell();
	void GetFreeCells(std::vector<std::pair<int, int>>& in);
	int GetFreeCellsCount();

};
