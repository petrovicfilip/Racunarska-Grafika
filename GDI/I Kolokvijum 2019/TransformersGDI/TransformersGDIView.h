
// TransformersGDIView.h : interface of the CTransformersGDIView class
//

#pragma once
#include "DImage.h"
#include <iostream>
using namespace std;

class CTransformersGDIView : public CView
{
protected: // create from serialization only
	CTransformersGDIView() noexcept;
	DECLARE_DYNCREATE(CTransformersGDIView)

// Attributes
public:
	CTransformersGDIDoc* GetDocument() const;
	DImage arm1;
	DImage arm2;
	DImage leg1;
	DImage leg2;
	DImage body;
	DImage background;

	double arm1rot = 0.0;
	double arm2rot = 0.0;
	double leg1rot = 0.0;
	double leg2rot = 0.0;
	double bodyrot = 0.0;


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawTransparent(CDC* pdc, DImage* img);
	virtual void DrawBackground(CDC* pDC, CRect rc);
	virtual void DrawArm1(CDC* pdc);
	virtual void DrawArm2(CDC* pdc);
	virtual void DrawLeg1(CDC* pdc);
	virtual void DrawLeg2(CDC* pdc);
	virtual void DrawBody(CDC* pdc);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTransformersGDIView();
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
	//afx_msg BOOL OnEraseBkgnd(CDC* pdc);
};

#ifndef _DEBUG  // debug version in TransformersGDIView.cpp
inline CTransformersGDIDoc* CTransformersGDIView::GetDocument() const
   { return reinterpret_cast<CTransformersGDIDoc*>(m_pDocument); }
#endif

