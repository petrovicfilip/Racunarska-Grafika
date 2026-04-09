
// klk2017najjaceView.h : interface of the Cklk2017najjaceView class
//
#include "DImage.h"
#pragma once


class Cklk2017najjaceView : public CView
{
protected: // create from serialization only
	Cklk2017najjaceView() noexcept;
	DECLARE_DYNCREATE(Cklk2017najjaceView)

// Attributes
public:
	Cklk2017najjaceDoc* GetDocument() const;
	DImage leg1;
	DImage leg2;
	DImage leg3;
	DImage body;
	DImage back2;
	double angBack = -20.0;
	double angFront = 0.0;
	double stepX = 0.0;
	double factor = 1.0;
	bool incBack = TRUE;
	bool incFront = TRUE;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawTransparent(CDC* pdc, DImage* img);
	virtual void DrawScene(CDC* pdc, CRect rect);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void Translate(CDC* pdc, float dX, float dY, bool rightMultiply);
	virtual void Rotate(CDC* pdc, float angle, bool rightMultiply);
	virtual void Scale(CDC* pdc, float sX, float sY, bool rightMultiply);
	virtual void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
// Implementation
public:
	virtual ~Cklk2017najjaceView();
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

#ifndef _DEBUG  // debug version in klk2017najjaceView.cpp
inline Cklk2017najjaceDoc* Cklk2017najjaceView::GetDocument() const
   { return reinterpret_cast<Cklk2017najjaceDoc*>(m_pDocument); }
#endif

