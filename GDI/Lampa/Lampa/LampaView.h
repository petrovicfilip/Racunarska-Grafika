
// LampaView.h : interface of the CLampaView class
//

#pragma once
#include "DImage.h"


class CLampaView : public CView
{
protected: // create from serialization only
	CLampaView() noexcept;
	DECLARE_DYNCREATE(CLampaView)

// Attributes
public:
	CLampaDoc* GetDocument() const;
	DImage arm1;
	DImage arm2;
	DImage base;
	DImage head;
	DImage arm1_shadow;
	DImage arm2_shadow;
	DImage base_shadow;
	DImage head_shadow;
	DImage pozadina;
	double arm1rot = 0.0;
	double arm2rot = 0.0;
	double headrot = 0.0;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawLamp(CDC* pdc, bool bIsShadow);
	virtual void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual void Rotate(CDC* pDC, float angle, bool rightMultiply);
	virtual void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	virtual void DrawBackground(CDC* pDC);
	void DrawTransparent(CDC* pdc, DImage* img);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawLampBase(CDC* pDC, bool bIsShadow);
	void DrawLampArm1(CDC* pDC, bool bIsShadow);
	void DrawLampArm2(CDC* pDC, bool bIsShadow);
	void DrawLampHead(CDC* pDC, bool bIsShadow);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLampaView();
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

#ifndef _DEBUG  // debug version in LampaView.cpp
inline CLampaDoc* CLampaView::GetDocument() const
   { return reinterpret_cast<CLampaDoc*>(m_pDocument); }
#endif

