
// Klk2024AView.h : interface of the CKlk2024AView class
//

#pragma once
#include "DImage.h"


class CKlk2024AView : public CView
{
protected: // create from serialization only
	CKlk2024AView() noexcept;
	DECLARE_DYNCREATE(CKlk2024AView)

// Attributes
public:
	CKlk2024ADoc* GetDocument() const;
	DImage* background;
	DImage* basket;
	DImage* arm;

	double offset = 0.0; // iz ovoga racuna ugao

	double alpha = -180;

	double beta = 0.0;

	double angle = -90;
// Operations
public:

// Overrides
public:
	void DrawBackground(CDC* pDC);
	void DrawArm(CDC* pdc);
	void DrawBasket(CDC* pdc, int r);
	void DrawBasketCouple(CDC* pdc, int l, int r, float angle);
	void DrawPlatform(CDC* pdc, int l, int r, float angle);
	void DrawCarousel(CDC* pdc, int l, int r, double offset, double alpha, double beta, double angle);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawImageTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2024AView();
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

#ifndef _DEBUG  // debug version in Klk2024AView.cpp
inline CKlk2024ADoc* CKlk2024AView::GetDocument() const
   { return reinterpret_cast<CKlk2024ADoc*>(m_pDocument); }
#endif

