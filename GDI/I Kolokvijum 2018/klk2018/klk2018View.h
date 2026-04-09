
// klk2018View.h : interface of the Cklk2018View class
//

#pragma once
#include "DImage.h"


class Cklk2018View : public CView
{
protected: // create from serialization only
	Cklk2018View() noexcept;
	DECLARE_DYNCREATE(Cklk2018View)

// Attributes
public:
	Cklk2018Doc* GetDocument() const;
	DImage* arm1;
	DImage* arm2;
	DImage* excavator;
	DImage* background;
	HENHMETAFILE fork;
	ENHMETAHEADER header;
	double x_offset = 0.0;
	double arm1_rot = 0.0;
	double arm2_rot = 0.0;
	double fork_rot = 0.0;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawFork(CDC* pdc);
	void DrawArm2(CDC* pdc);
	void DrawTransparent(CDC* pdc, DImage* img);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBody(CDC* pDC, CRect client);
	void DrawArm1(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cklk2018View();
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

#ifndef _DEBUG  // debug version in klk2018View.cpp
inline Cklk2018Doc* Cklk2018View::GetDocument() const
   { return reinterpret_cast<Cklk2018Doc*>(m_pDocument); }
#endif

