
// TransformersDrugiPutView.h : interface of the CTransformersDrugiPutView class
//

#pragma once
#include "DImage.h"

class CTransformersDrugiPutView : public CView
{
protected: // create from serialization only
	CTransformersDrugiPutView() noexcept;
	DECLARE_DYNCREATE(CTransformersDrugiPutView)

// Attributes
public:
	CTransformersDrugiPutDoc* GetDocument() const;
	DImage* arm1;
	DImage* arm2;
	DImage* leg1;
	DImage* leg2;
	DImage* body;
	DImage* background;
	double arm1rot = 0;
	double arm2rot = 0;
	double leg1rot = 0;
	double leg2rot = 0;
	double bodyrot = 0;
// Operations
public:

// Overrides
public:
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawArm2(CDC* pdc);
	void DrawLeg1(CDC* pdc);
	void DrawLeg2(CDC* pdc);
	void DrawBody1(CDC* pDC);
	void DrawTransformer(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawArm1(CDC* pdc);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTransformersDrugiPutView();
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

#ifndef _DEBUG  // debug version in TransformersDrugiPutView.cpp
inline CTransformersDrugiPutDoc* CTransformersDrugiPutView::GetDocument() const
   { return reinterpret_cast<CTransformersDrugiPutDoc*>(m_pDocument); }
#endif

