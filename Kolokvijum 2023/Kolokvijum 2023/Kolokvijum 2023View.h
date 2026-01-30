
// Kolokvijum 2023View.h : interface of the CKolokvijum2023View class
//

#pragma once
#include "DImage.h"


class CKolokvijum2023View : public CView
{
protected:
	// create from serialization only
	CKolokvijum2023View() noexcept;
	DECLARE_DYNCREATE(CKolokvijum2023View)

// Attributes
public:
	CKolokvijum2023Doc* GetDocument() const;
	/*glava.png,
	nadkolenica.png, nadlaktica.png, podkolenica.png, podlaktica.png, saka.png,
	stopalo.png, telo.png, pozadina.jpg*/
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* podkolenica;
	DImage* podlaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;
	DImage* glava;

	//za rotaciju nadlaktice, podlaktice, šake i celog
	//	robota, i za skaliranje celog robota
	double nadlakticarot = 0.0;
	double podlakticarot = 0.0;
	double sakarot = 0.0;
	double robotrot = 0.0;
	double factor = 1.0;


// Operations
public:

// Overrides
public:
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawHalf(CDC* pDC);
	void DrawHead(CDC* pDC);
	void DrawRobot(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKolokvijum2023View();
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

#ifndef _DEBUG  // debug version in Kolokvijum 2023View.cpp
inline CKolokvijum2023Doc* CKolokvijum2023View::GetDocument() const
   { return reinterpret_cast<CKolokvijum2023Doc*>(m_pDocument); }
#endif

