
// PacmanView.h : interface of the CPacmanView class
//

#pragma once
#include "DImage.h"

class CPacmanView : public CView
{
protected: // create from serialization only
	CPacmanView() noexcept;
	DECLARE_DYNCREATE(CPacmanView)

// Attributes
public:
	CPacmanDoc* GetDocument() const;
	DImage* background;
	bool bkdrawn = false;
	double x = 400;
	double y = 400;
	bool open_mouth = true;
	double angle = 60;
	long last_dir = VK_RIGHT;
	bool end = false;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pdc, double x, double y, bool rightmul);
	void DrawPacman(CDC* pdc, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPacmanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in PacmanView.cpp
inline CPacmanDoc* CPacmanView::GetDocument() const
   { return reinterpret_cast<CPacmanDoc*>(m_pDocument); }
#endif

